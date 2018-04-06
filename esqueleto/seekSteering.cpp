#include <stdafx.h>
#include "seekSteering.h"
#include "character.h"

void SeekSteering::GetSteering(Character& character, USVec2D& linearAcceleration, float& angularAcceleration) {
	mPosition = character.GetLoc();
	mDesiredVelociy = mtargetPosition - character.GetLoc();
	mDesiredVelociy.NormSafe();
	mDesiredVelociy = mDesiredVelociy * mMax_velocity;
	USVec2D desiredAcceleration = mDesiredVelociy - character.GetLinearVelocity();
	desiredAcceleration.NormSafe();
	mLinearAcceleration = desiredAcceleration * mMax_acceleration;
	linearAcceleration = mLinearAcceleration;
}

void SeekSteering::DrawDebug()
{
	//MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get();
	//
	//gfxDevice.SetPenColor(1.0f, 0.0f, 0.0f, 0.5f);
	//MOAIDraw::DrawLine(mPosition, mPosition + mDesiredVelociy);

	//gfxDevice.SetPenColor(0.0f, 0.0f, 1.0f, 0.5f);
	//MOAIDraw::DrawLine(mPosition, mPosition + mLinearAcceleration);
}