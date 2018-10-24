#include <stdafx.h>
#include "alignSteering.h"
#include "character.h"


void AlignSteering::GetSteering(Character& character, USVec2D& linearAcceleration, float& angularAcceleration) {
	//Get params
	float targetRotation         = character.GetParams().targetRotation;
	float maxAngularVelocity     = character.GetParams().max_angular_velocity;
	float maxAngularAcceleration = character.GetParams().max_angular_acceleration;
	float angularArriveRadius    = character.GetParams().angular_arrive_radius;
	

	//Calculate desired angular velocity
	float mRotation = character.GetRot();
	float mDesiredAngularVelociy = targetRotation - mRotation;

	//Convert desired angular velocity to [-PI, PI] interval
	if (mDesiredAngularVelociy > 180.0f) {
		mDesiredAngularVelociy -= 360.0f;
	}
	else if (mDesiredAngularVelociy < -180.0f) {
		mDesiredAngularVelociy += 360.0f;
	}

	//Apply arrive interpolation
	if (abs(mDesiredAngularVelociy) < angularArriveRadius) {
		mDesiredAngularVelociy = (mDesiredAngularVelociy / abs(mDesiredAngularVelociy)) * maxAngularVelocity * (abs(mDesiredAngularVelociy) / angularArriveRadius);
	}
	else {
		mDesiredAngularVelociy = (mDesiredAngularVelociy / abs(mDesiredAngularVelociy)) * maxAngularVelocity;
	}

	float mAngularAcceleration;
	if (mDesiredAngularVelociy > character.GetAngularVelocity())
		mAngularAcceleration = maxAngularAcceleration;
	else if (mDesiredAngularVelociy < character.GetAngularVelocity())
		mAngularAcceleration = -maxAngularAcceleration;
	else
		mAngularAcceleration = 0.0f;

	angularAcceleration = mAngularAcceleration;
}

void AlignSteering::DrawDebug()
{

}