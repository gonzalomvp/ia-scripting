#include <stdafx.h>
#include "character.h"
#include <tinyxml.h>

#include <params.h>
#include "seekSteering.h"
#include "arriveSteering.h"
#include "alignSteering.h"
#include "alignToMovementSteering.h"
#include "pursueSteering.h"
#include "pathFollowingSteering.h"
#include "stateMachine.h"

Character::Character() : mLinearVelocity(0.0f, 0.0f), mAngularVelocity(0.0f)
{
	RTTI_BEGIN
		RTTI_EXTEND (MOAIEntity2D)
	RTTI_END
}

Character::~Character()
{
	//TO-DO deletes
}

void Character::OnStart()
{
    ReadParams("params.xml", mParams);
	ReadPath("path.xml", mPath);
	//mSteerings.push_back(new SeekSteering(mParams));
	//mSteerings.push_back(new ArriveSteering(mParams));
	mSteerings.push_back(new AlignToMovementSteering(mParams));
	mSteerings.push_back(new PursueSteering(mParams));
	//mSteerings.push_back(new PathFollowingSteering(mParams, mPath));
	mEnemyPosition = USVec2D(0,0);
	mEnemySpeed = mParams.enemy_speed;

	m_stateMachine = new StateMachine(this);
	m_stateMachine->load();
	m_stateMachine->start();
}

void Character::OnStop()
{
	for (size_t i = 0; i < mSteerings.size(); i++) {
		delete mSteerings[i];
	}
	mSteerings.clear();
}

void Character::OnUpdate(float step)
{
	//Move enemy
	mEnemyPosition.mX += mEnemySpeed * step;
	if (mEnemyPosition.mX > mParams.enemy_maxPosition.mX)
	{
		mEnemyPosition.mX = mParams.enemy_maxPosition.mX;
		mEnemySpeed *= -1;
	}
	else if (mEnemyPosition.mX < mParams.enemy_minPosition.mX)
	{
		mEnemyPosition.mX = mParams.enemy_minPosition.mX;
		mEnemySpeed *= -1;
	}
	
	USVec2D linearAcceleration(0.0f, 0.0f);
	float angularAcceleration = 0.0f;

	for (size_t i = 0; i < mSteerings.size(); i++) {
		mSteerings[i]->GetSteering(*this, linearAcceleration, angularAcceleration);
	}

	mAngularVelocity += angularAcceleration * step;
	mLinearVelocity += linearAcceleration * step;

	//Move using linear velocity
	SetLoc(GetLoc() + mLinearVelocity * step);

	//Orientate sprite with velocity
	//SetRot(atan2(mLinearVelocity.mY , mLinearVelocity.mX) * 57.2958f);
	SetRot(GetRot() + mAngularVelocity * step);

	if (mEnemyPosition.DistSqrd(GetLoc()) < 400.0f)
	{
		SetLoc(USVec2D(0,0));
	}


	//StateMachine
	m_stateMachine->update();
}

void Character::DrawDebug()
{
	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get();
	gfxDevice.SetPenColor(0.0f, 0.0f, 1.0f, 0.5f);

	MOAIDraw::DrawPoint(mParams.targetPosition);

	for (size_t i = 0; i < mSteerings.size(); i++) {
		mSteerings[i]->DrawDebug();
	}
	//Draw current lineal velocity
	gfxDevice.SetPenColor(0.0f, 1.0f, 0.0f, 0.5f);
	MOAIDraw::DrawLine(GetLoc(), GetLoc() + mLinearVelocity);

	//Draw current lineal acceleration
	//MOAIDraw::DrawLine(GetLoc(), GetLoc() + mLinearVelocity);

	// Draw enemy
	gfxDevice.SetPenColor(1.0f, 0.0f, 0.0f, 1.0f);
	gfxDevice.SetPointSize(20.0f);
	MOAIDraw::DrawPoint(mEnemyPosition);
}





// Lua configuration

void Character::RegisterLuaFuncs(MOAILuaState& state)
{
	MOAIEntity2D::RegisterLuaFuncs(state);
	
	luaL_Reg regTable [] = {
		{ "setLinearVel",			_setLinearVel},
		{ "setAngularVel",			_setAngularVel},
		{ NULL, NULL }
	};

	luaL_register(state, 0, regTable);
}

int Character::_setLinearVel(lua_State* L)
{
	MOAI_LUA_SETUP(Character, "U")
	
	float pX = state.GetValue<float>(2, 0.0f);
	float pY = state.GetValue<float>(3, 0.0f);
	self->SetLinearVelocity(pX, pY);
	return 0;	
}

int Character::_setAngularVel(lua_State* L)
{
	MOAI_LUA_SETUP(Character, "U")
	
	float angle = state.GetValue<float>(2, 0.0f);
	self->SetAngularVelocity(angle);

	return 0;
}
	