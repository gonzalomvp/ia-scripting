#pragma once
#include <vector>

class Action;
class Transition;

class State {
public:
	State() : mEnterAction(nullptr), mExitAction(nullptr), mStateAction(nullptr) {}
	~State();

	virtual void onEnter(float step);
	virtual void update(float step);
	virtual void onExit(float step);

	const std::vector<Transition*>& getTransitions() { return mTransitions; }

	void setEnterAction(Action* action) { mEnterAction = action; }
	void setExitAction (Action* action) { mExitAction = action;  }
	void setStateAction(Action* action) { mStateAction = action; }

	void addTransition(Transition* transition) { mTransitions.push_back(transition); }

private:
	Action*                  mEnterAction;
	Action*                  mExitAction;
	Action*                  mStateAction;
	std::vector<Transition*> mTransitions;
};