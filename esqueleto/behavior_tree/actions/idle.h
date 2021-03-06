#pragma once
#include "behavior_tree/behavior.h"

class BehaviorTree;

class Idle : public Behavior {
public:
	Idle(BehaviorTree* owner) : Behavior(owner) {}

protected:
	virtual Status update(float step) override;
};
