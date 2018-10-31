#pragma once

#include "baseSteering.h"
#include "arriveSteering.h"

class PathFollowingSteering : public BaseSteering {
public:
	virtual void GetSteering(Character& character, USVec2D& linearAcceleration, float& angularAcceleration) override;
	virtual void DrawDebug() override;

private:
	ArriveSteering       mArriveSteering;
	std::vector<USVec2D> mPath;
	USVec2D              mClosestPoint; 
	int                  mClosestSegment;
	USVec2D              mFutureLoc;
};
