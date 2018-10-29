#pragma once
#include <vector>

class Character;
class State;

class StateMachine {
public:
	StateMachine(Character* owner) : m_owner(owner) {}
	void load();
	void start();
	void update(float step);
	Character* getCharacter() const { return m_owner; }

private:
	Character*          m_owner;
	std::vector<State*> m_States;
	State*              m_currentState;

};