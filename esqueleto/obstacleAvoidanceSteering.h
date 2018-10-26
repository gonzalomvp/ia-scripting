#pragma once

#include "baseSteering.h"

class ObstacleAvoidanceSteering : public BaseSteering {
public:
	virtual void GetSteering(Character& character, USVec2D& linearAcceleration, float& angularAcceleration) override;
	virtual void DrawDebug() override;

private:
	//USVec2D mDesiredVelociy;
	USVec2D mLinearAcceleration;
	USVec2D mPosition;
	std::vector<USVec3D> mObstacles;

	//Helper functions
	USVec2D calculateObstacleAvoidance(Character& character, USVec3D obstacle);
};
