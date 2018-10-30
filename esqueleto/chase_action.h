#pragma once
#include "action.h"

class AlignToMovementSteering;
class PursueSteering;

class ChaseAction : public Action {
public:
	ChaseAction(StateMachine* owner);
	~ChaseAction();
	virtual void start() override;
	virtual void end() override;

private:
	AlignToMovementSteering* mAlignToMovementSteering;
	PursueSteering* mPursueSteering;
};