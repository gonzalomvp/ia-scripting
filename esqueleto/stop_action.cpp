#include <stdafx.h>
#include "stop_action.h"

#include "character.h"
#include "stateMachine.h"

void StopAction::start() {
	mOwner->getCharacter()->SetLinearVelocity(0.0f, 0.0f);
	mOwner->getCharacter()->SetAngularVelocity(0.0f);
}