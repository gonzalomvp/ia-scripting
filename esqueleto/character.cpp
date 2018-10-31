#include <stdafx.h>
#include "character.h"
#include <tinyxml.h>

#include <params.h>
#include "steering_behaviors/alignSteering.h"
#include "steering_behaviors/alignToMovementSteering.h"
#include "steering_behaviors/arriveSteering.h"
#include "steering_behaviors/obstacleAvoidanceSteering.h"
#include "steering_behaviors/pathFollowingSteering.h"
#include "steering_behaviors/pursueSteering.h"
#include "steering_behaviors/seekSteering.h"
#include "state_machine/state_machine.h"
#include "behavior_tree/behavior_tree.h"

Character::Character() : mLinearVelocity(0.0f, 0.0f), mAngularVelocity(0.0f), mIsHit(false), mLife(5.0f), mStateMachine(nullptr), mBehaviorTree(nullptr)
{
	RTTI_BEGIN
		RTTI_EXTEND (MOAIEntity2D)
	RTTI_END
}

void Character::OnStart()
{
    ReadParams("params.xml", mParams);
	ReadPath("path.xml", mPath);
	ReadObstacles("obstacles.xml", mObstacles);
	//mSteerings.push_back(new AlignSteering());
	mSteerings.push_back(new AlignToMovementSteering());
	mSteerings.push_back(new ObstacleAvoidanceSteering());
	//mSteerings.push_back(new SeekSteering());
	//mSteerings.push_back(new ArriveSteering());
	//mSteerings.push_back(new PursueSteering());
	mSteerings.push_back(new PathFollowingSteering());
	
	mEnemyPosition = USVec2D(USFloat::Rand(-512, 512), USFloat::Rand(-384, 384));
	mEnemyTarget   = USVec2D(USFloat::Rand(-512, 512), USFloat::Rand(-384, 384));

	mStateMachine = new StateMachine(this);
	if (!mStateMachine->load("state_machine.xml")) {
		exit(1);
	}
	mStateMachine->start();

	mBehaviorTree = new BehaviorTree(this);
	if (!mBehaviorTree->load("behavior_tree.xml")) {
		exit(1);
	}
}

void Character::OnStop() {
	delete mStateMachine;
	delete mBehaviorTree;

	for (size_t i = 0; i < mSteerings.size(); ++i) {
		delete mSteerings[i];
	}
	mSteerings.clear();
}

void Character::OnUpdate(float step)
{
	////Move enemy
	//if (mEnemyPosition.DistSqrd(mEnemyTarget) <= 25.0f) {
	//	mEnemyTarget = USVec2D(USFloat::Rand(-512, 512), USFloat::Rand(-384, 384));
	//}
	//else {
	//	USVec2D movementDir = mEnemyTarget - mEnemyPosition;
	//	movementDir.NormSafe();
	//	mEnemyVelocity = movementDir * mParams.enemy_speed;
	//	mEnemyPosition += mEnemyVelocity * step;
	//}
	//if (mEnemyPosition.DistSqrd(GetLoc()) < 400.0f)
	//{
	//	SetLoc(USVec2D(USFloat::Rand(-512, 512), USFloat::Rand(-384, 384)));
	//	mEnemyPosition = USVec2D(USFloat::Rand(-512, 512), USFloat::Rand(-384, 384));
	//	mEnemyTarget = USVec2D(USFloat::Rand(-512, 512), USFloat::Rand(-384, 384));
	//}
	
	//Apply Steering Behaviors
	USVec2D linearAccelerationTotal(0.0f, 0.0f);
	float angularAccelerationTotal = 0.0f;

	for (size_t i = 0; i < mSteerings.size(); i++) {
		USVec2D linearAcceleration(0.0f, 0.0f);
		float angularAcceleration = 0.0f;
		mSteerings[i]->GetSteering(*this, linearAcceleration, angularAcceleration);
		linearAccelerationTotal += linearAcceleration;
		angularAccelerationTotal += angularAcceleration;
	}

	//Clamp accelerations
	if (linearAccelerationTotal.Length() > mParams.max_acceleration) {
		linearAccelerationTotal.NormSafe();
		linearAccelerationTotal = linearAccelerationTotal * mParams.max_acceleration;
	}
	if (angularAccelerationTotal != 0.0f && abs(angularAccelerationTotal) > mParams.max_angular_acceleration) {
		angularAccelerationTotal = (angularAccelerationTotal / abs(angularAccelerationTotal)) * mParams.max_angular_acceleration;
	}

	mLinearVelocity  += linearAccelerationTotal  * step;
	mAngularVelocity += angularAccelerationTotal * step;

	//Clamp velocities
	if (mLinearVelocity.Length() > mParams.max_velocity) {
		mLinearVelocity.NormSafe();
		mLinearVelocity = mLinearVelocity * mParams.max_velocity;
	}
	if (mAngularVelocity != 0.0f && abs(mAngularVelocity) > mParams.max_angular_velocity) {
		mAngularVelocity = (mAngularVelocity / abs(mAngularVelocity)) * mParams.max_angular_velocity;
	}

	//Move using linear velocity
	SetLoc(GetLoc() + mLinearVelocity * step);

	//Orientate sprite with velocity
	SetRot(GetRot() + mAngularVelocity * step);

	//StateMachine
	//mStateMachine->update(step);

	//BehaviorTree
	//mBehaviorTree->update(step);
}

void Character::DrawDebug()
{
	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get();

	for (size_t i = 0; i < mSteerings.size(); i++) {
		mSteerings[i]->DrawDebug();
	}
	//Draw current lineal velocity in green
	gfxDevice.SetPenColor(0.0f, 1.0f, 0.0f, 0.5f);
	MOAIDraw::DrawLine(GetLoc(), GetLoc() + mLinearVelocity);

	//Draw current lineal acceleration
	//MOAIDraw::DrawLine(GetLoc(), GetLoc() + mLinearVelocity);

	//// Draw enemy
	//gfxDevice.SetPenColor(1.0f, 0.0f, 0.0f, 1.0f);
	//gfxDevice.SetPointSize(20.0f);
	//MOAIDraw::DrawPoint(mEnemyPosition);
}

void Character::CheckHit(const USVec2D& clickPos) {
	if (clickPos.mX > GetLoc().mX - 32
		&& clickPos.mX  < GetLoc().mX + 32
		&& clickPos.mY > GetLoc().mY - 32
		&& clickPos.mY < GetLoc().mY + 32)
	{
		mIsHit = true;
	}
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
	
	self->CheckHit(USVec2D(pX, pY));

	return 0;
}
	