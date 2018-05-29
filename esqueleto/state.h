#pragma once
#include <vector>

class Action;
class Transition;

class State {
public:
	State() : m_enterAction(nullptr), m_exitAction(nullptr), m_stateAction(nullptr) {}

	virtual void onEnter();
	virtual void update();
	virtual void onExit();
	const std::vector<Transition*>& getTransitions() { return m_transitions; }

	void setEnterAction(Action* action) { m_enterAction = action; }
	void setExitAction (Action* action) { m_exitAction  = action; }
	void setStateAction(Action* action) { m_stateAction = action; }

	void addTransition(Transition* transition) { m_transitions.push_back(transition); }

private:
	Action*                  m_enterAction;
	Action*                  m_exitAction;
	Action*                  m_stateAction;
	std::vector<Transition*> m_transitions;
};