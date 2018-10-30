#include <stdafx.h>
#include "hit.h"
#include "character.h"
#include "behavior_tree/behavior_tree.h"

Status Hit::update(float step)
{
	mOwner->getCharacter()->SetLinearVelocity(0.0f, 0.0f);
	mOwner->getCharacter()->SetAngularVelocity(0.0f);
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
	mOwner->getCharacter()->SetIsHit(false);
	mOwner->getCharacter()->SetLife(mOwner->getCharacter()->GetLife() - 1.0f);
}