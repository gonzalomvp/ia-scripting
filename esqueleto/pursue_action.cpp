#include <stdafx.h>
#include "pursue_action.h"
#include "pursueSteering.h"
#include "character.h"
#include "stateMachine.h"

PursueAction::PursueAction(StateMachine* owner) : Action(owner)
{
	mPursueSteering = new PursueSteering(m_owner->getCharacter()->GetParams());
}

void PursueAction::start() {
	m_owner->getCharacter()->AddSteering(mPursueSteering);
}

void PursueAction::end() {
	m_owner->getCharacter()->RemoveSteering(mPursueSteering);
	m_owner->getCharacter()->SetLinearVelocity(0.0f, 0.0f);
	m_owner->getCharacter()->SetAngularVelocity(0.0f);
}