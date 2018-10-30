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
#include "obstacleAvoidanceSteering.h"
#include "stateMachine.h"
#include "behavior_tree.h"

Character::Character() : mLinearVelocity(0.0f, 0.0f), mAngularVelocity(0.0f), mIsHit(false), mLife(5.0f), mStateMachine(nullptr), mBehaviorTree(nullptr)
{
	RTTI_BEGIN
		RTTI_EXTEND (MOAIEntity2D)
	RTTI_END
}

Character::~Character()
{
	for (size_t i = 0; i < mSteerings.size(); ++i) {
		delete mSteerings[i];
	}
	delete mStateMachine;
	delete mBehaviorTree;
}


void Character::OnStart()
{
    ReadParams("params.xml", mParams);
	ReadPath("path.xml", mPath);
	//ReadObstacles("obstacles.xml", mObstacles);
	//mSteerings.push_back(new AlignSteering());
	//mSteerings.push_back(new AlignToMovementSteering());
	//mSteerings.push_back(new ObstacleAvoidanceSteering());
	//mSteerings.push_back(new SeekSteering());
	//mSteerings.push_back(new ArriveSteering());
	//mSteerings.push_back(new PursueSteering());
	//mSteerings.push_back(new PathFollowingSteering());
	
	mEnemyPosition = USVec2D(USFloat::Rand(-512, 512), USFloat::Rand(-384, 384));
	mEnemyTarget   = USVec2D(USFloat::Rand(-512, 512), USFloat::Rand(-384, 384));

	mStateMachine = new StateMachine(this);
	mStateMachine->load("state_machine.xml");
	mStateMachine->start();

	mBehaviorTree = new BehaviorTree(this);
	mBehaviorTree->load("behavior_tree.xml");
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
	if (mEnemyPosition.DistSqrd(mEnemyTarget) <= 25.0f) {
		mEnemyTarget = USVec2D(USFloat::Rand(-512, 512), USFloat::Rand(-384, 384));
	}
	else {
		USVec2D movementDir = mEnemyTarget - mEnemyPosition;
		movementDir.NormSafe();
		mEnemyVelocity = movementDir * mParams.enemy_speed;
		mEnemyPosition += mEnemyVelocity * step;
	}
	if (mEnemyPosition.DistSqrd(GetLoc()) < 400.0f)
	{
		SetLoc(USVec2D(USFloat::Rand(-512, 512), USFloat::Rand(-384, 384)));
		mEnemyPosition = USVec2D(USFloat::Rand(-512, 512), USFloat::Rand(-384, 384));
		mEnemyTarget = USVec2D(USFloat::Rand(-512, 512), USFloat::Rand(-384, 384));
	}
	
	//Apply Steering Behaviors
	USVec2D linearAcceleration(0.0f, 0.0f);
	USVec2D linearAccelerationAcc(0.0f, 0.0f);
	float angularAcceleration = 0.0f;

	for (size_t i = 0; i < mSteerings.size(); i++) {
		mSteerings[i]->GetSteering(*this, linearAcceleration, angularAcceleration);
		linearAccelerationAcc += linearAcceleration;
		if (linearAcceleration.LengthSquared() > 0.5f) {
			//break;
		}
	}
	if (linearAccelerationAcc.Length() > mParams.max_acceleration) {
		linearAccelerationAcc.NormSafe();
		linearAccelerationAcc = linearAccelerationAcc * mParams.max_acceleration;
	}

	mAngularVelocity += angularAcceleration * step;
	mLinearVelocity += linearAccelerationAcc * step;

	if (mLinearVelocity.Length() > mParams.max_velocity) {
		mLinearVelocity.NormSafe();
		mLinearVelocity = mLinearVelocity * mParams.max_velocity;
	}

	//Move using linear velocity
	SetLoc(GetLoc() + mLinearVelocity * step);

	//Orientate sprite with velocity
	//SetRot(atan2(mLinearVelocity.mY , mLinearVelocity.mX) * 57.2958f);
	SetRot(GetRot() + mAngularVelocity * step);

	//StateMachine
	mStateMachine->update(step);

	//BehaviorTree
	//mBehaviorTree->update(step);
}

void Character::DrawDebug()
{
	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get();
	gfxDevice.SetPenColor(1.0f, 0.0f, 0.0f, 0.5f);
	gfxDevice.SetPenWidth(1.0f);
	gfxDevice.SetPointSize(5.0f);

	MOAIDraw::DrawPoint(mParams.targetPosition);

	for (size_t i = 0; i < mSteerings.size(); i++) {
		mSteerings[i]->DrawDebug();
	}
	//Draw current lineal velocity in green
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
		{ "checkHit",			    _checkHit},
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

int Character::_checkHit(lua_State* L)
{
	MOAI_LUA_SETUP(Character, "U")

	float pX = state.GetValue<float>(2, 0.0f);
	float pY = state.GetValue<float>(3, 0.0f);
	
	float selfX = self->GetLoc().mX;
	float selfY = self->GetLoc().mY;

	if (pX > selfX - 32
		&& pX < selfX + 32
		&& pY > selfY - 32
		&& pY < selfY + 32)
	{
		self->mIsHit = true;
	}

	return 0;
}
	