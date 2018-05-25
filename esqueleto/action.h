#pragma once

class StateMachine;

class Action {
public:
	Action(StateMachine* owner) : m_owner(owner) {}

	virtual void start() {};
	virtual void update() {};
	virtual void end() {};

protected:
	StateMachine* m_owner;
};