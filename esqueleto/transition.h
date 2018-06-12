#pragma once

class Action;
class Condition;
class State;

class Transition {
public:
	Transition(Condition* condition, State* targetState, Action* triggerAction = nullptr) : m_condition(condition), m_targetState(targetState), m_triggerAction(triggerAction) {}
	bool canTrigger() const;
	State* trigger();

private:
	Condition* m_condition;
	State*     m_targetState;
	Action*    m_triggerAction;
};