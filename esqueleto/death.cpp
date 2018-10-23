#include <stdafx.h>
#include "death.h"
#include "character.h"
#include "behavior_tree.h"

Status Death::update(float step)
{
	m_owner->getCharacter()->SetLinearVelocity(0.0f, 0.0f);
	m_owner->getCharacter()->SetAngularVelocity(0.0f);
	m_owner->getCharacter()->SetImage(1);
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
	m_owner->getCharacter()->SetImage(0);
	m_owner->getCharacter()->SetLife(5.0f);
	m_owner->getCharacter()->SetLoc(USVec2D(0, 0));
}