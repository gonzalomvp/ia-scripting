#include <stdafx.h>
#include "is_close.h"
#include "character.h"
#include "behavior_tree.h"

Status IsClose::update(float step)
{
	float sqrdDist = m_owner->getCharacter()->GetEnemyPosition().DistSqrd(m_owner->getCharacter()->GetLoc());
	if (sqrdDist < (m_MinDistance * m_MinDistance))
	{
		return eSuccess;
	}
	else
	{
		return eFail;
	}
}
