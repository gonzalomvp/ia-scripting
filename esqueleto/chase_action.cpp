#include <stdafx.h>
#include "chase_action.h"
#include "pursueSteering.h"
#include "alignToMovementSteering.h"
#include "character.h"
#include "stateMachine.h"

ChaseAction::ChaseAction(StateMachine* owner) : Action(owner) {
	mPursueSteering = new PursueSteering();
	mAlignToMovementSteering = new AlignToMovementSteering();
}

ChaseAction::~ChaseAction() {
	delete mPursueSteering;
	delete mAlignToMovementSteering;
}

void ChaseAction::start() {
	mOwner->getCharacter()->AddSteering(mPursueSteering);
	mOwner->getCharacter()->AddSteering(mAlignToMovementSteering);
}

void ChaseAction::end() {
	mOwner->getCharacter()->RemoveSteering(mPursueSteering);
	mOwner->getCharacter()->RemoveSteering(mAlignToMovementSteering);
}