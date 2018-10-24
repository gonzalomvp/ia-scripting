#pragma once
#include "behavior.h"

class BehaviorTree;

class Idle : public Behavior
{
public:
	Idle(BehaviorTree* owner) : Behavior(owner) {}

protected:
	virtual void onEnter() override;
	virtual Status update(float step) override;
};