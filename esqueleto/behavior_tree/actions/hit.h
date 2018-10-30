#pragma once
#include "behavior_tree/behavior.h"

class BehaviorTree;

class Hit : public Behavior
{
public:
	Hit(BehaviorTree* owner) : Behavior(owner) {}

protected:
	virtual Status update(float step) override;
	virtual void onEnter() override;
	virtual void onExit() override;
	float m_hitTimer;
};
