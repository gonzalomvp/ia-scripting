#pragma once

class StateMachine;

class Condition {
public:
	Condition(StateMachine* owner) : mOwner(owner) {}
	virtual bool check() const = 0;

protected:
	StateMachine* mOwner;
};
