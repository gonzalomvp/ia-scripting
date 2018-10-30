#pragma once
#include "behavior.h"

class BehaviorTree;
class AlignToMovementSteering;
class PursueSteering;

class Pursue : public Behavior
{
public:
	Pursue(BehaviorTree* owner, float arriveDistance);

protected:
	virtual void onEnter() override;
	virtual Status update(float step) override;
	virtual void onExit() override;

private:
	AlignToMovementSteering * mAlignToMovementSteering;
	PursueSteering * mPursueSteering;
	float mArriveDistance;
};
