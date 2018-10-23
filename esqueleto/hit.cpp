#include <stdafx.h>
#include "hit.h"
#include "character.h"
#include "behavior_tree.h"

Status Hit::update(float step)
{
	m_owner->getCharacter()->SetLinearVelocity(0.0f, 0.0f);
	m_owner->getCharacter()->SetAngularVelocity(0.0f);
	m_owner->getCharacter()->SetImage(4);
	m_hitTimer += step;
	if (m_hitTimer >= 0.5f)
	{
		return eSuccess;
	}
	else
	{
		return eRunning;
	}
}

void Hit::onEnter()
{
	m_hitTimer = 0;

}

void Hit::onExit()
{
	m_owner->getCharacter()->SetImage(0);

	m_owner->getCharacter()->SetIsHit(false);
	m_owner->getCharacter()->SetLife(m_owner->getCharacter()->GetLife() - 1.0f);
}