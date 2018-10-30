#pragma once

#include "state_machine/condition.h"

class NotCondition : public Condition {
public:
	NotCondition(StateMachine* owner, Condition* condition) : Condition(owner), mCondition(condition) {}
	virtual bool check() const override { return !mCondition->check(); }

protected:
	Condition* mCondition;
};