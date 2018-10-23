#pragma once
#include "behavior.h"
#include <vector>

class BehaviorTree;

class Group : public Behavior
{
public:
	Group(BehaviorTree* owner) : Behavior(owner) {}
	void AddBehavior(Behavior* behavior) { m_Children.push_back(behavior); }

protected:
	typedef std::vector<Behavior*> Behaviors;
	Behaviors m_Children;
};