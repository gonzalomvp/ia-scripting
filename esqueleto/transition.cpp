#include <stdafx.h>
#include "transition.h"

#include "action.h"
#include "condition.h"

bool Transition::canTrigger() const {
	return m_condition->check();
}

State* Transition::trigger() { 
	if (m_triggerAction){
		m_triggerAction->start();
		m_triggerAction->update();
		m_triggerAction->end();
	}
	return m_targetState;
}