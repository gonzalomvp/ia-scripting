#include <stdafx.h>
#include "stateMachine.h"

#include"change_sprite_action.h"
#include"pursue_action.h"
#include "hit_action.h"
#include "hit_condition.h"
#include "dead_condition.h"
#include "death_timer.h"
#include "state.h"
#include "transition.h"
#include "distance_condition.h"
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

	State* hit = new State();
	Action* hitAnim = new ChangeSpriteAction(this, 4);
	Action* hitTimer = new HitAction(this);
	hit->setEnterAction(hitAnim);
	hit->setStateAction(hitTimer);
	m_States.push_back(hit);

	State* death = new State();
	Action* deathAnim = new ChangeSpriteAction(this, 1);
	Action* deathTimer = new DeathTimer(this);
	death->setEnterAction(deathAnim);
	death->setStateAction(deathTimer);
	m_States.push_back(death);


	Transition* onKilled = new Transition(new DeadCondition(this), death);
	idle->addTransition(onKilled);

	Transition* inPursueRange = new Transition(new DistanceCondition(this, 200.0f), alarm);
	idle->addTransition(inPursueRange);

	Transition* outOfRange = new Transition(new NotCondition(this, new DistanceCondition(this, 200.0f)), idle);
	alarm->addTransition(outOfRange);

	Transition* hitStart = new Transition(new HitCondition(this), hit);
	idle->addTransition(hitStart);
	alarm->addTransition(hitStart);

	Transition* hitEnd = new Transition(new NotCondition(this, new HitCondition(this)), idle);
	hit->addTransition(hitEnd);

	Transition* respawn = new Transition(new NotCondition(this, new DeadCondition(this)), idle);
	death->addTransition(respawn);
}

void StateMachine::start()
{
	m_currentState = m_States[0];
	m_currentState->onEnter(0);
}

void StateMachine::update(float step) {
	m_currentState->update(step);
	const std::vector<Transition*>& transitions = m_currentState->getTransitions();
	for (size_t i = 0; i < transitions.size(); i++) {
		Transition* transition = transitions[i];
		if (transition->canTrigger()) {
			m_currentState->onExit(step);
			State* nextState = transition->trigger(step);
			nextState->onEnter(step);
			m_currentState = nextState;
			return;
		}
	}
}


