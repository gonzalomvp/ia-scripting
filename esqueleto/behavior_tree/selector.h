#pragma once
#include "group.h"

class BehaviorTree;

class Selector : public Group
{
public:
	Selector(BehaviorTree* owner) : Group(owner) {}

protected:
	void onEnter();
	Status update(float step);

	int mCurrentChild;
};
