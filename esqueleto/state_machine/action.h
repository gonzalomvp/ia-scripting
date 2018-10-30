#pragma once

class StateMachine;

class Action {
public:
	Action(StateMachine* owner) : mOwner(owner) {}

	virtual void start() {};
	virtual void update(float step) {};
	virtual void end() {};

protected:
	StateMachine* mOwner;
};