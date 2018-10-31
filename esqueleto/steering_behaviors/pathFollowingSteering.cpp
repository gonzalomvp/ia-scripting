#include <stdafx.h>
#include "pathFollowingSteering.h"
#include "arriveSteering.h"
#include "character.h"
#include "utils.h"


void PathFollowingSteering::GetSteering(Character& character, USVec2D& linearAcceleration, float& angularAcceleration) {
	//Get params
	mPath = character.GetPath();
	float lookAhead = character.GetParams().look_ahead;
	float timeAhead = character.GetParams().time_ahead;

	//Calculate future location
	mFutureLoc = character.GetLoc() + character.GetLinearVelocity() * timeAhead;
	
	//Calculate the closest point and segment in the path from the future location
	closestPointToSegments(mFutureLoc, mPath, mClosestPoint, mClosestSegment);

	//Move ahead the closest point and apply the arrive steering
	character.GetParams().targetPosition = lookAheadInPath(mClosestPoint, mPath, lookAhead);
	mArriveSteering.GetSteering(character, linearAcceleration, angularAcceleration);
}

void PathFollowingSteering::DrawDebug()
{
	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get();
	gfxDevice.SetPenWidth(2.0f);
	gfxDevice.SetPointSize(5.0f);
	//Draw path
	for (size_t i = 0; i + 1 < mPath.size(); ++i) {
		//Closest segment in green
		if (i == mClosestSegment) {
			gfxDevice.SetPenColor(0.0f, 1.0f, 0.0f, 0.0f);
		}
		//Other segments in red
		else {
			gfxDevice.SetPenColor(1.0f, 0.0f, 0.0f, 0.0f);
		}
		MOAIDraw::DrawLine(mPath[i], mPath[i + 1]);
	}

	//Draw closest point in green
	gfxDevice.SetPenColor(0.0f, 1.0f, 0.0f, 0.0f);
	MOAIDraw::DrawPoint(mClosestPoint);

	//Draw future location in blue
	gfxDevice.SetPenColor(0.0f, 0.0f, 1.0f, 0.0f);
	MOAIDraw::DrawPoint(mFutureLoc);

	//Delegated steering debug
	mArriveSteering.DrawDebug();
}

