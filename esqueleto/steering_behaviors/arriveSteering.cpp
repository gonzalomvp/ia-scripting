#include <stdafx.h>
#include "arriveSteering.h"
#include "character.h"

void ArriveSteering::GetSteering(Character& character, USVec2D& linearAcceleration, float& angularAcceleration) {
	//Get params
	mTargetPosition       = character.GetParams().targetPosition;
	mArriveRadius         = character.GetParams().arrive_radius;
	float maxVelocity     = character.GetParams().max_velocity;
	float maxAcceleration = character.GetParams().max_acceleration;

	//Calculate desired velocity
	mPosition = character.GetLoc();
	mDesiredVelociy = mTargetPosition - character.GetLoc();
	float distance = mDesiredVelociy.Length();
	mDesiredVelociy.NormSafe();
	if (distance < mArriveRadius) {
		mDesiredVelociy = mDesiredVelociy * maxVelocity * (distance / mArriveRadius);
	}
	else {
		mDesiredVelociy = mDesiredVelociy * maxVelocity;
	}

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

void ArriveSteering::DrawDebug() {
	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get();
	gfxDevice.SetPenWidth(1.0f);
	gfxDevice.SetPointSize(1.0f);

	//Draw desired velocity in red
	gfxDevice.SetPenColor(1.0f, 0.0f, 0.0f, 0.5f);
	MOAIDraw::DrawLine(mPosition, mPosition + mDesiredVelociy);

	//Draw linear acceleration in blue
	gfxDevice.SetPenColor(0.0f, 0.0f, 1.0f, 0.5f);
	MOAIDraw::DrawLine(mPosition, mPosition + mLinearAcceleration);

	//Draw arrive area in red
	gfxDevice.SetPenColor(1.0f, 0.0f, 0.0f, 0.5f);
	MOAIDraw::DrawEllipseOutline(mTargetPosition.mX, mTargetPosition.mY, mArriveRadius, mArriveRadius, 20);

	//Draw target in red
	gfxDevice.SetPenColor(1.0f, 0.0f, 0.0f, 0.5f);
	gfxDevice.SetPointSize(5.0f);
	MOAIDraw::DrawPoint(mTargetPosition);
}