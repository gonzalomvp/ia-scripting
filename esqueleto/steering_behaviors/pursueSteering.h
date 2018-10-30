#pragma once

#include "baseSteering.h"
#include "arriveSteering.h"

class PursueSteering : public BaseSteering
{
public:
	PursueSteering() : mArriveSteering() {}
	virtual void GetSteering(Character& character, USVec2D& linearAcceleration, float& angularAcceleration);
	virtual void DrawDebug();

private:
	ArriveSteering mArriveSteering;
	//float mMaxVelocity;
};