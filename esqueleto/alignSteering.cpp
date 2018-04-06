#include <stdafx.h>
#include "alignSteering.h"
#include "character.h"


void AlignSteering::GetSteering(Character& character, USVec2D& linearAcceleration, float& angularAcceleration) {
	mRotation = character.GetRot();
	mDesiredAngularVelociy = mTargetRotation - mRotation;

	//clamp
	if (mDesiredAngularVelociy > 180.0f) {
		mDesiredAngularVelociy -= 2 * 180.0f;
	}
	else if (mDesiredAngularVelociy < -180.0f) {
		mDesiredAngularVelociy += 2 * 180.0f;
	}

	abs(mDesiredAngularVelociy);

	mDesiredAngularVelociy = (mDesiredAngularVelociy / abs(mDesiredAngularVelociy)) * mMax_angular_velocity;
	
	if (mDesiredAngularVelociy > character.GetAngularVelocity())
		mAngularAcceleration = mMax_angular_acceleration;
	else if (mDesiredAngularVelociy < character.GetAngularVelocity())
		mAngularAcceleration = -mMax_angular_acceleration;
	else
		mAngularAcceleration = 0.0f;

	angularAcceleration = mAngularAcceleration;
}

void AlignSteering::DrawDebug()
{

}