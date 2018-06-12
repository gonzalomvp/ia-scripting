#include <stdafx.h>
#include "stateMachine.h"

#include"change_sprite_action.h"
#include "state.h"
#include "transition.h"

void StateMachine::load() {
	State* idle = new State();
	Action* idleAction = new ChangeSpriteAction(this, 2);
	idle->setEnterAction(idleAction);
	m_States.push_back(idle);

	State* attack = new State();
	Action* attackAction = new ChangeSpriteAction(this, 1);
	attack->setEnterAction(idleAction);
	m_States.push_back(attack);
}

void StateMachine::start()
{
	m_currentState = m_States[0];
	m_currentState->onEnter();
}

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


