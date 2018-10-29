#pragma once
#include "action.h"

class DeathTimer : public Action {
public:
	DeathTimer(StateMachine* owner) : Action(owner) {}
	virtual void start() override;
	virtual void update(float step) override;

private:
	float mDeathTimer;
};