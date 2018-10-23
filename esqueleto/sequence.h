#pragma once
#include "group.h"

class BehaviorTree;

class Sequence : public Group
{
public:
	Sequence(BehaviorTree* owner) : Group(owner) {}

protected:
	void onEnter();
	Status update(float step);
	int m_CurrentChild;
};
