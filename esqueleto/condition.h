#pragma once

class StateMachine;

class Condition {
public:
	Condition(StateMachine* owner) : m_owner(owner) {}
	virtual bool check() const = 0;

protected:
	StateMachine* m_owner;
};
