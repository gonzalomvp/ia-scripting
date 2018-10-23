#include <stdafx.h>
#include "is_dead.h"
#include "character.h"
#include "behavior_tree.h"

Status IsDead::update(float step)
{
	if (m_owner->getCharacter()->GetLife() <= 0.0f)
	{
		return eSuccess;
	}
	else
	{
		return eFail;
	}
}
