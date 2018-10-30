#include <stdafx.h>
#include "death.h"
#include "character.h"
#include "behavior_tree/behavior_tree.h"

Status Death::update(float step)
{
	mOwner->getCharacter()->SetLinearVelocity(0.0f, 0.0f);
	mOwner->getCharacter()->SetAngularVelocity(0.0f);
	m_respawnTimer += step;
	if (m_respawnTimer >= 5.0f)
	{
		return eSuccess;
	}
	else
	{
		return eRunning;
	}
}

void Death::onEnter()
{
	m_respawnTimer = 0;

}

void Death::onExit()
{
	mOwner->getCharacter()->SetLife(5.0f);
	mOwner->getCharacter()->SetLoc(USVec2D(0, 0));
}