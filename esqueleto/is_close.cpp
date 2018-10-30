#include <stdafx.h>
#include "is_close.h"
#include "character.h"
#include "behavior_tree.h"

Status IsClose::update(float step)
{
	float sqrdDist = mOwner->getCharacter()->GetEnemyPosition().DistSqrd(mOwner->getCharacter()->GetLoc());
	if (sqrdDist < (m_MinDistance * m_MinDistance))
	{
		return eSuccess;
	}
	else
	{
		return eFail;
	}
}
