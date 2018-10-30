#include <stdafx.h>
#include "check_distance.h"
#include "character.h"
#include "behavior_tree/behavior_tree.h"

Status CheckDistance::update(float step) {
	Character* character = mOwner->getCharacter();
	USVec2D    enemyPos  = character->GetEnemyPosition();

	if (enemyPos.DistSqrd(character->GetLoc()) < (mMinDistance * mMinDistance)) {
		return eSuccess;
	}

	return eFail;
}
