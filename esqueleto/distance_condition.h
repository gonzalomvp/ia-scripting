#pragma once

#include "condition.h"

class DistanceCondition : public Condition {
public:
	DistanceCondition(StateMachine* owner, float minDistance) : Condition(owner), mMinDistance(minDistance) {}
	virtual bool check() const override;

private:
	float mMinDistance;
};