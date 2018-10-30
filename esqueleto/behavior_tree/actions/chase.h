#pragma once
#include "behavior_tree/behavior.h"

class BehaviorTree;
class AlignToMovementSteering;
class PursueSteering;

class Chase : public Behavior {
public:
	Chase(BehaviorTree* owner, float arriveDistance, float lostDistance);
	~Chase();

protected:
	virtual void onEnter() override;
	virtual Status update(float step) override;
	virtual void onExit() override;

private:
	float                    mArriveDistance;
	float                    mLostDistance;
	AlignToMovementSteering* mAlignToMovementSteering;
	PursueSteering*          mPursueSteering;
};
