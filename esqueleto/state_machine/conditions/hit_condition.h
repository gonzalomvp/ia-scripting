#pragma once

#include "state_machine/condition.h"

class HitCondition : public Condition {
public:
	HitCondition(StateMachine* owner) : Condition(owner) {}
	virtual bool check() const override;
};