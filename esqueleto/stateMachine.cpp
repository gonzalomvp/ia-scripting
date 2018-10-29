#include <stdafx.h>
#include "stateMachine.h"

#include "change_sprite_action.h"
#include "pursue_action.h"
#include "stop_action.h"
#include "hit_action.h"
#include "hit_condition.h"
#include "dead_condition.h"
#include "death_timer.h"
#include "state.h"
#include "transition.h"
#include "distance_condition.h"
#include "notCondition.h"

#include <tinyxml.h>

bool StateMachine::load(const char* filename) {
	TiXmlDocument doc(filename);
	if (!doc.LoadFile())
	{
		fprintf(stderr, "Couldn't read state machine configuration from %s", filename);
		return false;
	}

	TiXmlHandle hDoc(&doc);

	TiXmlElement* pElem;
	pElem = hDoc.FirstChildElement().Element();
	if (!pElem)
	{
		fprintf(stderr, "Invalid format for %s", filename);
		return false;
	}

	TiXmlHandle hRoot(pElem);

	TiXmlHandle hParams = hRoot.FirstChildElement("states").FirstChildElement("state");
	TiXmlElement* stateElem = hParams.Element();
	for (stateElem; stateElem; stateElem = stateElem->NextSiblingElement()) {
		State* state = new State();
		
		TiXmlElement* actionElem = stateElem->FirstChildElement("enter_action");
		Action* action = createAction(actionElem);
		if (action) {
			state->setEnterAction(action);
		}

		actionElem = stateElem->FirstChildElement("state_action");
		action = createAction(actionElem);
		if (action) {
			state->setStateAction(action);
		}

		actionElem = stateElem->FirstChildElement("exit_action");
		action = createAction(actionElem);
		if (action) {
			state->setExitAction(action);
		}

		m_States.push_back(state);
	}

	hParams = hRoot.FirstChildElement("transitions").FirstChildElement("transition");
	TiXmlElement* transitionElem = hParams.Element();
	for (transitionElem; transitionElem; transitionElem = transitionElem->NextSiblingElement()) {
		int initialState;
		int endState;
		transitionElem->Attribute("initial_state", &initialState);
		transitionElem->Attribute("end_state", &endState);
		TiXmlElement* conditionElem = transitionElem->FirstChildElement("condition");
		Condition* condition = createCondition(conditionElem);
		if (condition) {
			Transition* transition = new Transition(condition, m_States[endState]);
			m_States[initialState]->addTransition(transition);
		}
	}

	return true;
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

Action* StateMachine::createAction(TiXmlElement* actionElem) {
	Action* action = nullptr;
	if (actionElem) {
		std::string type = actionElem->Attribute("type");
		std::vector<std::string> params;
		TiXmlElement* paramElem = actionElem->FirstChildElement("param");
		for (paramElem; paramElem; paramElem = paramElem->NextSiblingElement()) {
			params.push_back(paramElem->Attribute("value"));
		}

		if (type == "changeSprite") {
			action = new ChangeSpriteAction(this, std::stoi(params[0]));
		}
		else if (type == "stop") {
			action = new StopAction(this);
		}
		else if (type == "pursue") {
			action = new PursueAction(this);
		}
		else if (type == "hit") {
			action = new HitAction(this);
		}
		else if (type == "death") {
			action = new DeathTimer(this);
		}
	}
	
	return action;
}

Condition* StateMachine::createCondition(TiXmlElement* conditionElem) {
	Condition* condition = nullptr;
	if (conditionElem) {
		std::string type = conditionElem->Attribute("type");
		std::vector<std::string> params;
		TiXmlElement* paramElem = conditionElem->FirstChildElement("param");
		for (paramElem; paramElem; paramElem = paramElem->NextSiblingElement()) {
			params.push_back(paramElem->Attribute("value"));
		}

		if (type == "dead") {
			condition = new DeadCondition(this);
		}
		else if (type == "hit") {
			condition = new HitCondition(this);
		}
		else if (type == "distanceLessThan") {
			condition = new DistanceCondition(this, std::stoi(params[0]));
		}
		else if (type == "not") {
			TiXmlElement* notConditionElem = conditionElem->FirstChildElement("condition");
			condition = new NotCondition(this, createCondition(notConditionElem));
		}
	}

	return condition;
}


