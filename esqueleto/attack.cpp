#include <stdafx.h>
#include "attack.h"
#include "character.h"
#include "behavior_tree.h"

Status Attack::update(float step)
{
	mOwner->getCharacter()->SetLinearVelocity(0.0f, 0.0f);
	mOwner->getCharacter()->SetAngularVelocity(0.0f);
	return eSuccess;
}