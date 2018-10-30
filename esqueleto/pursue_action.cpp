#include <stdafx.h>
#include "pursue_action.h"
#include "pursueSteering.h"
#include "alignToMovementSteering.h"
#include "character.h"
#include "stateMachine.h"

PursueAction::PursueAction(StateMachine* owner) : Action(owner) {
	mPursueSteering = new PursueSteering();
	mAlignToMovementSteering = new AlignToMovementSteering();
}

PursueAction::~PursueAction() {
	delete mPursueSteering;
	delete mAlignToMovementSteering;
}

void PursueAction::start() {
	mOwner->getCharacter()->AddSteering(mPursueSteering);
	mOwner->getCharacter()->AddSteering(mAlignToMovementSteering);
}

void PursueAction::update(float step) {

}

void PursueAction::end() {
	mOwner->getCharacter()->RemoveSteering(mPursueSteering);
	mOwner->getCharacter()->RemoveSteering(mAlignToMovementSteering);
}