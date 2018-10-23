#include <stdafx.h>
#include "distanceCondition.h"

#include "character.h"
#include "stateMachine.h"


bool DistanceCondition::check() const{
	Character* character = m_owner->getCharacter();
	USVec2D enemyPos = character->GetEnemyPosition();

	if (enemyPos.DistSqrd(character->GetLoc()) < 40000.0f)
	{
		return true;
	}

	return false;
}