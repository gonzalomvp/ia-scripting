#include <stdafx.h>
#include "idle.h"
#include "character.h"
#include "behavior_tree.h"

void Idle::onEnter()
{
	
}

Status Idle::update(float step)
{
	m_owner->getCharacter()->SetLinearVelocity(0.0f, 0.0f);
	m_owner->getCharacter()->SetAngularVelocity(0.0f);
	m_owner->getCharacter()->SetImage(0);
	return eRunning;
}