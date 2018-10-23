#pragma once

#include "condition.h"

class NotCondition : public Condition {
public:
	NotCondition(StateMachine* owner, Condition* condition) : Condition(owner), mCondition(condition) {}
	virtual bool check() const { return !mCondition->check(); }

protected:
	Condition* mCondition;
};