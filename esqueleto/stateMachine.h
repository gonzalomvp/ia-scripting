#pragma once
#include <vector>

class Action;
class Condition;
class Character;
class State;
class TiXmlElement;

class StateMachine {
public:
	StateMachine(Character* owner) : mOwner(owner) {}
	~StateMachine();

	bool load(const char* filename);
	Action* createAction(TiXmlElement* actionElem);
	Condition* createCondition(TiXmlElement* conditionElem);
	void start();
	void update(float step);
	Character* getCharacter() const { return mOwner; }

private:
	Character*          mOwner;
	std::vector<State*> mStates;
	State*              mCurrentState;

};