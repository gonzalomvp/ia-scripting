#include <stdafx.h>
#include "character.h"
#include <tinyxml.h>

#include <params.h>
#include "seekSteering.h"
#include "arriveSteering.h"
#include "alignSteering.h"
#include "alignToMovementSteering.h"
#include "pathFollowingSteering.h"

Character::Character() : mLinearVelocity(0.0f, 0.0f), mAngularVelocity(0.0f)
{
	RTTI_BEGIN
		RTTI_EXTEND (MOAIEntity2D)
	RTTI_END
}

Character::~Character()
{

}

void Character::OnStart()
{
    ReadParams("params.xml", mParams);
	ReadPath("path.xml", mPath);
	//mSteerings.push_back(new ArriveSteering(mParams));
	//mSteerings.push_back(new AlignToMovementSteering(mParams));
	mSteerings.push_back(new PathFollowingSteering(mParams, mPath));
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
	