#pragma once
#include "state_machine/action.h"

class IdleAction : public Action {
public:
	IdleAction(StateMachine* owner) : Action(owner) {}
	virtual void start() override;
};