#pragma once
#include <vector>

class Character;
class State;

class StateMachine {
public:
	void load();
	void start();
	void update();
	Character* getOwner() const { return m_owner; }

private:
	Character*          m_owner;
	std::vector<State*> m_States;
	State*              m_currentState;

};