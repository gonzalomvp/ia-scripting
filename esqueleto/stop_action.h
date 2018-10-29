#pragma once
#include "action.h"

class StopAction : public Action {
public:
	StopAction(StateMachine* owner) : Action(owner) {}
	virtual void start();
};