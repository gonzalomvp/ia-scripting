#pragma once

#include "baseSteering.h"

class ArriveSteering : public BaseSteering {
public:
	virtual void GetSteering(Character& character,  USVec2D& linearAcceleration, float& angularAcceleration) override;
	virtual void DrawDebug() override;

private:
	//Debug variables
	USVec2D mDesiredVelociy;
	USVec2D mLinearAcceleration;
	USVec2D mPosition;
	USVec2D mTargetPosition;
	float   mArriveRadius;
};