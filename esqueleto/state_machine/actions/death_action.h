#pragma once
#include "state_machine/action.h"

class DeathAction : public Action {
public:
	DeathAction(StateMachine* owner) : Action(owner) {}
	virtual void start() override;
	virtual void update(float step) override;

private:
	float mDeathTimer;
};