#pragma once
#include "action.h"

class PursueSteering;

class PursueAction : public Action {
public:
	PursueAction(StateMachine* owner);
	virtual void start() override;
	virtual void end() override;

protected:
	PursueSteering* mPursueSteering;
};