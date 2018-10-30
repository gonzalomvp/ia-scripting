#pragma once
#include "action.h"

class AlignToMovementSteering;
class PursueSteering;

class PursueAction : public Action {
public:
	PursueAction(StateMachine* owner);
	~PursueAction();
	virtual void start() override;
	virtual void update(float step) override;
	virtual void end() override;

private:
	AlignToMovementSteering* mAlignToMovementSteering;
	PursueSteering* mPursueSteering;
};