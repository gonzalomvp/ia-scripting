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

	//Helper functions
	void closestPointInPath(const USVec2D& currentPosition, USVec2D& closestPoint, int& closestSegment);
	USVec2D closestPointInSegment(const USVec2D& currentPosition, const USVec2D& segmentStart, const USVec2D& segmentEnd);
	USVec2D lookAheadFromPoint(const USVec2D& point, float lookAhead);
};
