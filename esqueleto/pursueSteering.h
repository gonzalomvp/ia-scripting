#pragma once

#include "baseSteering.h"
#include "arriveSteering.h"

class PursueSteering : public BaseSteering
{
public:
	PursueSteering(Params params) : mArriveSteering(params), mMaxVelocity(params.max_velocity) {}
	virtual void GetSteering(Character& character, USVec2D& linearAcceleration, float& angularAcceleration);
	virtual void DrawDebug();

private:
	ArriveSteering mArriveSteering;
	float mMaxVelocity;
};