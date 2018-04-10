#pragma once

#include "baseSteering.h"
#include "arriveSteering.h"

class PathFollowingSteering : public BaseSteering {
public:
	PathFollowingSteering(Params params, const std::vector<USVec2D>& path) : mArriveSteering(params), mLook_ahead(params.look_ahead), mPath(path) {}
	virtual void GetSteering(Character& character, USVec2D& linearAcceleration, float& angularAcceleration);
	virtual void DrawDebug();

private:
	void closestPointInPath(const USVec2D& currentPosition, USVec2D& closestPoint, int& closestSegment);
	USVec2D closestPointInSegment(const USVec2D& currentPosition, const USVec2D& segmentStart, const USVec2D& segmentEnd);
	USVec2D lookAheadFromPoint(const USVec2D& point, float lookAhead);

	ArriveSteering mArriveSteering;
	const std::vector<USVec2D> mPath;
	USVec2D mClosestPoint; 
	int mClosestSegment;
	USVec2D mLookAheadPoint;
	float mLook_ahead;
};
