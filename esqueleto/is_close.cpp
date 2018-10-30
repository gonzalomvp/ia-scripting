#include <stdafx.h>
#include "is_close.h"
#include "character.h"
#include "behavior_tree.h"

Status IsClose::update(float step)
{
	Character* character = mOwner->getCharacter();
	USVec2D enemyPos = character->GetEnemyPosition();

	if (enemyPos.DistSqrd(character->GetLoc()) < (mMinDistance * mMinDistance)) {
		return eSuccess;
	}

	return eFail;
}
