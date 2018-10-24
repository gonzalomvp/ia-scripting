#pragma once

#include "baseSteering.h"

class AlignSteering : public BaseSteering {
public:
	virtual void GetSteering(Character& character, USVec2D& linearAcceleration, float& angularAcceleration) override;
	virtual void DrawDebug() override;

private:
	//float mDesiredAngularVelociy;
	//float mAngularAcceleration;
	//float mRotation;

	//float mTargetRotation;
	//float mMax_angular_velocity;
	//float mMax_angular_acceleration;
	//float mAngular_arrive_radius;
};
