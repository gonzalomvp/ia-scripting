#pragma once
#include "behavior.h"

class BehaviorTree;

class IsDead : public Behavior
{
public:
	IsDead(BehaviorTree* owner) : Behavior(owner) {}

protected:
	virtual Status update(float step) override;
};