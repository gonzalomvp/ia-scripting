#pragma once

#include "condition.h"

class DistanceCondition : public Condition {
public:
	DistanceCondition(StateMachine* owner) : Condition(owner) {}
	virtual bool check() const;
};