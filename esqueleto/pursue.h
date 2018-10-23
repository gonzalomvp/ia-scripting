#pragma once
#include "behavior.h"

class BehaviorTree;
class PursueSteering;

class Pursue : public Behavior
{
public:
	Pursue(BehaviorTree* owner);

protected:
	virtual void onEnter() override;
	virtual Status update(float step) override;
	virtual void onExit() override;

private:
	PursueSteering * mPursueSteering;
};
