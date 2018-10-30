#pragma once
#include "action.h"

class IdleAction : public Action {
public:
	IdleAction(StateMachine* owner) : Action(owner) {}
	virtual void start() override;
};