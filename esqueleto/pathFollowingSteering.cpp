#include <stdafx.h>
#include "pathFollowingSteering.h"
#include "arriveSteering.h"
#include "character.h"


void PathFollowingSteering::GetSteering(Character& character, USVec2D& linearAcceleration, float& angularAcceleration) {
	//calcular punto mas cercano
	closestPointInPath(character.GetLoc(), mClosestPoint, mClosestSegment);

	//calcular punto avanzado
	mLookAheadPoint = lookAheadFromPoint(character.GetLoc(), mLook_ahead);

	mArriveSteering.setTargetPosition(mPath[0]);
	mArriveSteering.GetSteering(character, linearAcceleration, angularAcceleration);
}

void PathFollowingSteering::DrawDebug()
{
	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get();
	gfxDevice.SetPenColor(1.0f, 0.0f, 0.0f, 0.0f);

	for (size_t i = 0; i < mPath.size() -1 ; i++)
	{
		if (i == mClosestSegment)
			gfxDevice.SetPenColor(0.0f, 1.0f, 0.0f, 0.0f);
		else
			gfxDevice.SetPenColor(1.0f, 0.0f, 0.0f, 0.0f);
		MOAIDraw::DrawLine(mPath[i], mPath[i + 1]);
	}

	gfxDevice.SetPenColor(0.0f, 1.0f, 0.0f, 0.0f);
	//gfxDevice.SetPointSize(5.0f);
	gfxDevice.SetPointSize(5.0f);
	MOAIDraw::DrawPoint(mClosestPoint);
	gfxDevice.SetPenColor(0.0f, 0.0f, 1.0f, 0.0f);
	MOAIDraw::DrawPoint(mLookAheadPoint);
}

void PathFollowingSteering::closestPointInPath(const USVec2D& currentPosition, USVec2D& closestPoint, int& closestSegment) {
	int closestSegmentIndex = -1;
	float minDistance = 99999999999;
	for (size_t i = 0; i < mPath.size() - 1; i++)
	{
		USVec2D point = closestPointInSegment(currentPosition, mPath[i], mPath[i + 1]);
		float distance = (point - currentPosition).Length();
		if (distance < minDistance) {
			minDistance = distance;
			closestSegment = i;
			closestPoint = point;
		}
	}
}

USVec2D PathFollowingSteering::closestPointInSegment(const USVec2D& currentPosition, const USVec2D& segmentStart, const USVec2D& segmentEnd) {
	USVec2D closestPoint;
	
	USVec2D np = (segmentEnd - segmentStart);
	np.NormSafe();
	USVec2D r0 = currentPosition - segmentStart;
	float proj = r0.Dot(np);
	if (proj > (segmentEnd - segmentStart).Length() ) {
		closestPoint = segmentEnd;
	}

	else if (proj < 0) {
		closestPoint = segmentStart;
	}
	
	else {
		closestPoint = segmentStart + (np * proj);
	}


	return closestPoint;
}

USVec2D PathFollowingSteering::lookAheadFromPoint(const USVec2D& point, float lookAhead) {
	USVec2D lookAheadPoint = mPath[mPath.size() - 1];
	float totalAhead = 0.0f;
	int segmentIndex = mClosestSegment;

	USVec2D segmentStart = point;
	USVec2D segmentEnd = mPath[mClosestSegment + 1];
	float segmentLength = (segmentEnd - segmentStart).Length();

	while (segmentIndex < mPath.size() - 1) {
		if (totalAhead + segmentLength < lookAhead) {
			totalAhead += segmentLength;
			++segmentIndex;
		}
		else {
			USVec2D segmentDir = segmentEnd - segmentStart;
			segmentDir.NormSafe();
			lookAheadPoint = segmentStart + segmentDir * (lookAhead - totalAhead);
			break;
		}
	}


	return lookAheadPoint;
}