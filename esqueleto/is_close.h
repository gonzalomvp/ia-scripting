#pragma once
#include "behavior.h"

class BehaviorTree;

class IsClose : public Behavior
{
public:
	IsClose(BehaviorTree* owner, float minDistance) : Behavior(owner), m_MinDistance(minDistance) {}

protected:
	virtual Status update(float step) override;

private:
	float m_MinDistance;
};