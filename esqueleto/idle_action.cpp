#include <stdafx.h>
#include "idle_action.h"

#include "character.h"
#include "stateMachine.h"

void IdleAction::start() {
	mOwner->getCharacter()->SetLinearVelocity(0.0f, 0.0f);
	mOwner->getCharacter()->SetAngularVelocity(0.0f);
}