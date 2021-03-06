#include <stdafx.h>
#include "state_machine/state_machine.h"
#include "state_machine/state.h"
#include "state_machine/transition.h"
#include "state_machine/actions/change_sprite_action.h"
#include "state_machine/actions/chase_action.h"
#include "state_machine/actions/death_action.h"
#include "state_machine/actions/hit_action.h"
#include "state_machine/actions/idle_action.h"
#include "state_machine/conditions/dead_condition.h"
#include "state_machine/conditions/distance_condition.h"
#include "state_machine/conditions/hit_condition.h"
#include "state_machine/conditions/not_condition.h"

#include <tinyxml.h>

StateMachine::~StateMachine() {
	for (size_t i = 0; i < mStates.size(); ++i) {
		delete mStates[i];
	}
}

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

		mStates.push_back(state);
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
			Transition* transition = new Transition(condition, mStates[endState]);
			mStates[initialState]->addTransition(transition);
		}
	}

	return true;
}

void StateMachine::start()
{
	mCurrentState = mStates[0];
	mCurrentState->onEnter(0);
}

void StateMachine::update(float step) {
	mCurrentState->update(step);
	const std::vector<Transition*>& transitions = mCurrentState->getTransitions();
	for (size_t i = 0; i < transitions.size(); ++i) {
		Transition* transition = transitions[i];
		if (transition->canTrigger()) {
			mCurrentState->onExit(step);
			State* nextState = transition->trigger(step);
			nextState->onEnter(step);
			mCurrentState = nextState;
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
		else if (type == "idle") {
			action = new IdleAction(this);
		}
		else if (type == "chase") {
			action = new ChaseAction(this);
		}
		else if (type == "hit") {
			action = new HitAction(this);
		}
		else if (type == "death") {
			action = new DeathAction(this);
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
			condition = new DistanceCondition(this, std::stof(params[0]));
		}
		else if (type == "not") {
			TiXmlElement* notConditionElem = conditionElem->FirstChildElement("condition");
			condition = new NotCondition(this, createCondition(notConditionElem));
		}
	}

	return condition;
}


