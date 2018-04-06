#pragma once

#include "baseSteering.h"

class ArriveSteering : public BaseSteering {
public:
	ArriveSteering(const Params& params) : mtargetPosition(params.targetPosition), mMax_velocity(params.max_velocity), mMax_acceleration(params.max_acceleration), mArrive_radius(params.arrive_radius) {}
	virtual void GetSteering(Character& character,  USVec2D& linearAcceleration, float& angularAcceleration);
	virtual void DrawDebug();

	void setTargetPosition(const USVec2D& targetPosition) { mtargetPosition = targetPosition; }

private:
	USVec2D mDesiredVelociy;
	USVec2D mLinearAcceleration;
	USVec2D mPosition;

	USVec2D mtargetPosition;
	float mMax_velocity;
	float mMax_acceleration;
	float mArrive_radius;
};