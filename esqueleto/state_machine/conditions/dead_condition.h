#pragma once

#include "state_machine/condition.h"

class DeadCondition : public Condition {
public:
	DeadCondition(StateMachine* owner) : Condition(owner) {}
	virtual bool check() const override;
};