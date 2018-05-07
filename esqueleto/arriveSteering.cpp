#include <stdafx.h>
#include "arriveSteering.h"
#include "character.h"

void ArriveSteering::GetSteering(Character& character, USVec2D& linearAcceleration, float& angularAcceleration) {
	mPosition = character.GetLoc();
	mDesiredVelociy = mtargetPosition - character.GetLoc();

	float distance = mDesiredVelociy.Length();
	mDesiredVelociy.NormSafe();
	if (distance < mArrive_radius) {
		mDesiredVelociy = mDesiredVelociy * mMax_velocity * (distance / mArrive_radius);
	}

	else {
		
		mDesiredVelociy = mDesiredVelociy * mMax_velocity;
	}

	USVec2D desiredAcceleration = mDesiredVelociy - character.GetLinearVelocity();
	desiredAcceleration.NormSafe();
	mLinearAcceleration = desiredAcceleration * mMax_acceleration;
	linearAcceleration = mLinearAcceleration;
	printf("velocity: %f\n", mDesiredVelociy.Length());
}

void ArriveSteering::DrawDebug()
{
	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get();

	gfxDevice.SetPenColor(1.0f, 0.0f, 0.0f, 0.5f);
	MOAIDraw::DrawLine(mPosition, mPosition + mDesiredVelociy);

	gfxDevice.SetPenColor(0.0f, 0.0f, 1.0f, 0.5f);
	MOAIDraw::DrawLine(mPosition, mPosition + mLinearAcceleration);

	gfxDevice.SetPenColor(0.0f, 1.0f, 0.0f, 0.5f);
	gfxDevice.SetPointSize(5.0f);
	MOAIDraw::DrawPoint(mtargetPosition);
}