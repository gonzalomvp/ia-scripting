#pragma once
#include "behavior.h"

class BehaviorTree;

class Attack : public Behavior
{
public:
	Attack(BehaviorTree* owner) : Behavior(owner) {}

protected:
	virtual Status update(float step) override;
};
