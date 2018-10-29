#include <stdafx.h>
#include "dead_condition.h"

#include "character.h"
#include "stateMachine.h"

bool DeadCondition::check() const {
	return mOwner->getCharacter()->GetLife() <= 0.0f;
}