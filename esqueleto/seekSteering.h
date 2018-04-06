#pragma once

#include "baseSteering.h"

class SeekSteering : public BaseSteering {
public:
	SeekSteering(const Params& params) : mtargetPosition(params.targetPosition), mMax_velocity(params.max_velocity), mMax_acceleration(params.max_acceleration) {}
	virtual void GetSteering(Character& character, USVec2D& linearAcceleration, float& angularAcceleration);
	virtual void DrawDebug();

	void setTargetPosition(const USVec2D& targetPosition) { mtargetPosition = targetPosition; }

private:
	USVec2D mDesiredVelociy;
	USVec2D mLinearAcceleration;
	USVec2D mPosition;

	USVec2D mtargetPosition;
	float mMax_velocity;
	float mMax_acceleration;
};