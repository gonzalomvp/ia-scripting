#include <stdafx.h>
#include "is_dead.h"
#include "character.h"
#include "behavior_tree/behavior_tree.h"

Status IsDead::update(float step)
{
	if (mOwner->getCharacter()->GetLife() <= 0.0f)
	{
		return eSuccess;
	}
	else
	{
		return eFail;
	}
}
