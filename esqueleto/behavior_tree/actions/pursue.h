#pragma once
#include "behavior_tree/behavior.h"

class BehaviorTree;
class AlignToMovementSteering;
class PursueSteering;

class Pursue : public Behavior
{
public:
	Pursue(BehaviorTree* owner, float arriveDistance, float lostDistance);
	~Pursue();

protected:
	virtual void onEnter() override;
	virtual Status update(float step) override;
	virtual void onExit() override;

private:
	AlignToMovementSteering* mAlignToMovementSteering;
	PursueSteering* mPursueSteering;
	float mArriveDistance;
	float mLostDistance;
};
