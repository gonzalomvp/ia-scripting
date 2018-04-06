#pragma once

#pragma once

#include "baseSteering.h"

class AlignSteering : public BaseSteering {
public:
	AlignSteering(Params params) : mTargetRotation(params.targetRotation), mMax_angular_velocity(params.max_angular_velocity), mMax_angular_acceleration(params.max_angular_acceleration) {}
	virtual void GetSteering(Character& character, USVec2D& linearAcceleration, float& angularAcceleration);
	virtual void DrawDebug();

	void setTargetRotation(float targetRotation) { mTargetRotation = targetRotation; }

private:
	float mDesiredAngularVelociy;
	float mAngularAcceleration;
	float mRotation;

	float mTargetRotation;
	float mMax_angular_velocity;
	float mMax_angular_acceleration;
};
