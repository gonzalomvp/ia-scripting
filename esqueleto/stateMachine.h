#pragma once
#include <vector>

class Action;
class Condition;
class Character;
class State;
class TiXmlElement;

class StateMachine {
public:
	StateMachine(Character* owner) : m_owner(owner) {}
	bool load(const char* filename);
	Action* createAction(TiXmlElement* actionElem);
	Condition* createCondition(TiXmlElement* conditionElem);
	void start();
	void update(float step);
	Character* getCharacter() const { return m_owner; }

private:
	Character*          m_owner;
	std::vector<State*> m_States;
	State*              m_currentState;

};