#include <stdafx.h>
#include "seekSteering.h"
#include "character.h"

void SeekSteering::GetSteering(Character& character, USVec2D& linearAcceleration, float& angularAcceleration) {
	//Get params
	mTargetPosition          = character.GetParams().targetPosition;
	float   maxVelocity     = character.GetParams().max_velocity;
	float   maxAcceleration = character.GetParams().max_acceleration;
	
	//Calculate desired velocity
	mPosition = character.GetLoc();
	mDesiredVelociy = mTargetPosition - character.GetLoc();
	mDesiredVelociy.NormSafe();
	mDesiredVelociy = mDesiredVelociy * maxVelocity;

	//Calculate desired acceleration
	mLinearAcceleration = mDesiredVelociy - character.GetLinearVelocity();
	if (mLinearAcceleration.LengthSquared() > 0.5f) {
		mLinearAcceleration.NormSafe();
		mLinearAcceleration = mLinearAcceleration * maxAcceleration;
	}
	//Don't accelerate if desired velocity and current velocity are very similar
	else {
		mLinearAcceleration = USVec2D(0.0f, 0.0f);
	}
	
	linearAcceleration = mLinearAcceleration;
}

void SeekSteering::DrawDebug() {
	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get();
	gfxDevice.SetPenWidth(1.0f);
	gfxDevice.SetPointSize(1.0f);
	
	//Draw desired velocity in red
	gfxDevice.SetPenColor(1.0f, 0.0f, 0.0f, 0.5f);
	MOAIDraw::DrawLine(mPosition, mPosition + mDesiredVelociy);

	//Draw linear acceleration in blue
	gfxDevice.SetPenColor(0.0f, 0.0f, 1.0f, 0.5f);
	MOAIDraw::DrawLine(mPosition, mPosition + mLinearAcceleration);

	//Draw target in red
	gfxDevice.SetPenColor(1.0f, 0.0f, 0.0f, 0.5f);
	gfxDevice.SetPointSize(5.0f);
	MOAIDraw::DrawPoint(mTargetPosition);
}