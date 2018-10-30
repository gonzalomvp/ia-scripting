#include <stdafx.h>
#include "distance_condition.h"
#include "character.h"
#include "state_machine/state_machine.h"

bool DistanceCondition::check() const{
	Character* character = mOwner->getCharacter();
	USVec2D    enemyPos  = character->GetEnemyPosition();

	if (enemyPos.DistSqrd(character->GetLoc()) < (mMinDistance * mMinDistance)) {
		return true;
	}

	return false;
}