#include <stdafx.h>
#include "transition.h"

#include "action.h"
#include "condition.h"

Transition::~Transition() {
	delete mCondition;
	delete mTriggerAction;
}

bool Transition::canTrigger() const {
	return mCondition->check();
}

State* Transition::trigger(float step) { 
	if (mTriggerAction){
		mTriggerAction->start();
		mTriggerAction->update(step);
		mTriggerAction->end();
	}
	return mTargetState;
}