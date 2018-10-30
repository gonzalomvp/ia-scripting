#pragma once

#include "baseSteering.h"

#define M_PIf 3.14159265358979323846f
#define RAD2DEG(_rad_) (360.f * (_rad_) / (2.f * M_PIf))
#define DEG2RAD(_deg_) ((2.f * M_PIf) * (_deg_) / 360.f)

class AlignSteering : public BaseSteering {
public:
	virtual void GetSteering(Character& character, USVec2D& linearAcceleration, float& angularAcceleration) override;
	virtual void DrawDebug() override {}
};
