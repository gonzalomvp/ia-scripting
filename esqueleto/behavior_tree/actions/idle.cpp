#include <stdafx.h>
#include "idle.h"
#include "character.h"
#include "behavior_tree/behavior_tree.h"

void Idle::onEnter()
{
	
}

Status Idle::update(float step)
{
	mOwner->getCharacter()->SetLinearVelocity(0.0f, 0.0f);
	mOwner->getCharacter()->SetAngularVelocity(0.0f);
	return eSuccess;
}