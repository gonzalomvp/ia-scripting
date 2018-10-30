#pragma once

class Action;
class Condition;
class State;

class Transition {
public:
	Transition(Condition* condition, State* targetState, Action* triggerAction = nullptr) : mCondition(condition), mTargetState(targetState), mTriggerAction(triggerAction) {}
	~Transition();

	bool canTrigger() const;
	State* trigger(float step);

private:
	Condition* mCondition;
	State*     mTargetState;
	Action*    mTriggerAction;
};