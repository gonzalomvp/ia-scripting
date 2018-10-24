#pragma once

#include "baseSteering.h"
#include "alignSteering.h"

class AlignToMovementSteering : public BaseSteering {
public:
	virtual void GetSteering(Character& character, USVec2D& linearAcceleration, float& angularAcceleration) override;
	virtual void DrawDebug() override {}

private:
	AlignSteering mAlignSteering;
};
