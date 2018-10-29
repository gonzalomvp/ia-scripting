#include <stdafx.h>
#include "hit_condition.h"

#include "character.h"
#include "stateMachine.h"


bool HitCondition::check() const {
	return mOwner->getCharacter()->GetIsHit();
}