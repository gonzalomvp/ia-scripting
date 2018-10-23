#include <stdafx.h>
#include "attack.h"
#include "character.h"
#include "behavior_tree.h"

Status Attack::update(float step)
{
	m_owner->getCharacter()->SetLinearVelocity(0.0f, 0.0f);
	m_owner->getCharacter()->SetAngularVelocity(0.0f);
	m_owner->getCharacter()->SetImage(2);
	return eSuccess;
}