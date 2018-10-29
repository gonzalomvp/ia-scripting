#pragma once
#include "action.h"

class HitAction : public Action {
public:
	HitAction(StateMachine* owner) : Action(owner) {}
	virtual void start() override;
	virtual void update(float step) override;

private:
	float mHitTimer;
};