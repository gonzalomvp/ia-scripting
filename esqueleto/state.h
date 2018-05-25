#pragma once
#include <vector>

class Action;
class Transition;

class State {
public:
	virtual void onEnter() {}
	virtual void update() {}
	virtual void onExit() {}
	const std::vector<Transition*>& getTransitions() { return m_transitions; }

private:
	Action*                  m_enterAction;
	Action*                  m_exitAction;
	Action*                  m_stateAction;
	std::vector<Transition*> m_transitions;
};