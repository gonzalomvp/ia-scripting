#include <stdafx.h>
#include "obstacleAvoidanceSteering.h"
#include "character.h"

void ObstacleAvoidanceSteering::GetSteering(Character& character, USVec2D& linearAcceleration, float& angularAcceleration) {
	//Get params
	mObstacles            = character.GetObstacles();
	mPosition             = character.GetLoc();
	//float maxVelocity     = character.GetParams().max_velocity;
	//float maxAcceleration = character.GetParams().max_acceleration;

	//mDesiredVelociy = USVec2D(0.0f, 0.0f);
	mLinearAcceleration = USVec2D(0.0f, 0.0f);

	//calculate desired velocity to avoid each obstacle
	for (size_t i = 0; i < mObstacles.size(); ++i) {
		mLinearAcceleration += calculateObstacleAvoidance(character, i);
	}

	//if (!mDesiredVelociy.Equals(USVec2D(0.0f, 0.0f))) {
	//	mDesiredVelociy.NormSafe();
	//	mDesiredVelociy = mDesiredVelociy * maxVelocity;

	//	//Calculate desired acceleration
	//	mLinearAcceleration = mDesiredVelociy - character.GetLinearVelocity();
	//	if (mLinearAcceleration.LengthSquared() > 0.5f) {
	//		mLinearAcceleration.NormSafe();
	//		mLinearAcceleration = mLinearAcceleration * maxAcceleration;
	//	}
	//}

	linearAcceleration = mLinearAcceleration;
}

void ObstacleAvoidanceSteering::DrawDebug()
{
	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get();
	gfxDevice.SetPenWidth(1.0f);
	gfxDevice.SetPointSize(1.0f);

	//Draw obstacles
	for (size_t i = 0; i < mObstacles.size(); ++i) {
		USVec2D obstacleLoc(mObstacles[i].mX, mObstacles[i].mY);
		float obstacleRadius = mObstacles[i].mZ;

		gfxDevice.SetPenColor(1.0f, 1.0f, 1.0f, 0.5f);
		if (std::find(mCollisionObstacles.begin(), mCollisionObstacles.end(), i) != mCollisionObstacles.end()) {
			gfxDevice.SetPenColor(1.0f, 0.0f, 0.0f, 1.0f);
		}
		MOAIDraw::DrawEllipseFill(obstacleLoc.mX, obstacleLoc.mY, obstacleRadius, obstacleRadius, 20);
	}
	mCollisionObstacles.clear();

	//Draw desired velocity in red
	//gfxDevice.SetPenColor(1.0f, 0.0f, 0.0f, 0.5f);
	//MOAIDraw::DrawLine(mPosition, mPosition + mDesiredVelociy);

	//Draw linear acceleration in blue
	gfxDevice.SetPenColor(0.0f, 0.0f, 1.0f, 0.5f);
	MOAIDraw::DrawLine(mPosition, mPosition + mLinearAcceleration);
}

USVec2D ObstacleAvoidanceSteering::calculateObstacleAvoidance(Character& character, int obstacleIndex) {
	USVec3D obstacle = mObstacles[obstacleIndex];
	USVec2D linearAcceleration(0.0f, 0.0f);

	USVec2D obstacleLoc(obstacle.mX, obstacle.mY);
	float obstacleRadius = obstacle.mZ;
	
	USVec2D velocity = character.GetLinearVelocity();
	velocity.NormSafe();

	USVec2D lookAhead = velocity * 50.0f;

	USVec2D obstacleDirection = obstacleLoc - character.GetLoc();
	float projection = obstacleDirection.Dot(velocity);
	
	USVec2D closestPoint = character.GetLoc() + velocity * projection;

	if (projection > lookAhead.Length()) {
		closestPoint = character.GetLoc() + lookAhead;
	}

	float dist = (closestPoint - obstacleLoc).Length();

	if (dist < obstacleRadius + character.GetParams().char_radius && projection > 0.0f) {
		
		float penetrationFactor = 1 - (dist / (obstacleRadius + character.GetParams().char_radius));

		float avoidanceDirection = obstacleDirection.Cross(lookAhead);
		mCollisionObstacles.push_back(obstacleIndex);
		linearAcceleration = obstacleDirection;
		linearAcceleration.NormSafe();
		linearAcceleration = linearAcceleration * (100 + (900 * penetrationFactor));
		if (avoidanceDirection > 0.0f) {
			linearAcceleration.Rotate90Clockwise();
		}
		else {
			linearAcceleration.Rotate90Anticlockwise();
		}

	}

	return linearAcceleration;
}