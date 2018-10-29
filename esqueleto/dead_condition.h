#pragma once

#include "condition.h"

class DeadCondition : public Condition {
public:
	DeadCondition(StateMachine* owner) : Condition(owner) {}
	virtual bool check() const;
};