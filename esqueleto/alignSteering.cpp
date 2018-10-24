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
	float rotation = character.GetRot();
	float desiredAngularVelociy = targetRotation - rotation;

	//Convert desired angular velocity to [-PI, PI] interval
	if (desiredAngularVelociy > 180.0f) {
		desiredAngularVelociy -= 360.0f;
	}
	else if (desiredAngularVelociy < -180.0f) {
		desiredAngularVelociy += 360.0f;
	}

	//Apply arrive interpolation
	float velocityAbs = abs(desiredAngularVelociy);
	float velocitySign = 0.0f;
	if (velocityAbs != 0.0f) {
		velocitySign = desiredAngularVelociy / velocityAbs;
	}
	if (velocityAbs < angularArriveRadius) {
		desiredAngularVelociy = velocitySign * maxAngularVelocity * (velocityAbs / angularArriveRadius);
	}
	else {
		desiredAngularVelociy = velocitySign * maxAngularVelocity;
	}

	// Calculate desired angular acceleration
	angularAcceleration = desiredAngularVelociy - character.GetAngularVelocity();
	if (angularAcceleration != 0.0f) {
		angularAcceleration = (angularAcceleration / abs(angularAcceleration)) * maxAngularAcceleration;
	}

	angularAcceleration = angularAcceleration;
}
