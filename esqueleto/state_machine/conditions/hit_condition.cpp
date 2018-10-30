#include <stdafx.h>
#include "hit_condition.h"
#include "character.h"
#include "state_machine/state_machine.h"

bool HitCondition::check() const {
	return mOwner->getCharacter()->GetIsHit();
}