#include <stdafx.h>
#include "check_hit.h"
#include "character.h"
#include "behavior_tree.h"

Status CheckHit::update(float step)
{
	if (m_owner->getCharacter()->GetIsHit())
	{
		return eSuccess;
	}
	else
	{
		return eFail;
	}
}
