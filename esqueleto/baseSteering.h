#pragma once
#include "params.h"

class Character;

class BaseSteering {
public:
	virtual void GetSteering(Character& character, USVec2D& linearAcceleration, float& angularAcceleration) = 0;
	virtual void DrawDebug() = 0;
};

