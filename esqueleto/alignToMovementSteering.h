#pragma once

#include "baseSteering.h"
#include "alignSteering.h"

class AlignToMovementSteering : public BaseSteering {
public:
	//AlignToMovementSteering(Params params) : mAlignSteering(params) {}
	virtual void GetSteering(Character& character, USVec2D& linearAcceleration, float& angularAcceleration);
	virtual void DrawDebug();

private:
	
	float mDesiredAngularVelociy;
	float mAngularAcceleration;
	float mRotation;

	AlignSteering mAlignSteering;
};
