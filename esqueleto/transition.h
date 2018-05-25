#pragma once

class Action;
class Condition;
class State;

class Transition {
public:
	bool canTrigger() const;
	State* trigger();

private:
	Condition* m_condition;
	State*     m_targetState;
	Action*    m_triggerAction;
};