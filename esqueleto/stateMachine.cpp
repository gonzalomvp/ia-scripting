#include <stdafx.h>
#include "stateMachine.h"

#include"change_sprite_action.h"
#include"pursue_action.h"
#include "state.h"
#include "transition.h"
#include "distanceCondition.h"
#include "notCondition.h"

void StateMachine::load() {
	State* idle = new State();
	Action* idleAction = new ChangeSpriteAction(this, 0);
	idle->setEnterAction(idleAction);
	m_States.push_back(idle);

	State* alarm = new State();
	Action* alarmAction = new ChangeSpriteAction(this, 3);
	alarm->setEnterAction(alarmAction);
	Action* pursueAction = new PursueAction(this);
	alarm->setStateAction(pursueAction);
	m_States.push_back(alarm);

	Transition* closeTransition = new Transition(new DistanceCondition(this), alarm);
	idle->addTransition(closeTransition);

	Transition* farTransition = new Transition(new NotCondition(this, new DistanceCondition(this)), idle);
	alarm->addTransition(farTransition);
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


