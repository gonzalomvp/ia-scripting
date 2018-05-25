#include "stateMachine.h"
#include "state.h"
#include "transition.h"

void StateMachine::update() {
	m_currentState->update();
	const std::vector<Transition*>& transitions = m_currentState->getTransitions();
	for (size_t i = 0; i < transitions.size(); i++) {
		Transition* transition = transitions[i];
		if (transition->canTrigger()) {
			m_currentState->onExit();
			State* nextState = transition->trigger();
			nextState->onEnter();
			m_currentState = nextState;
			return;
		}
	}
}