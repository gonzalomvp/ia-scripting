#include <stdafx.h>
#include "pursue.h"
#include "alignToMovementSteering.h"
#include "pursueSteering.h"
#include "character.h"
#include "behavior_tree.h"

Pursue::Pursue(BehaviorTree* owner) : Behavior(owner)
{
	mPursueSteering = new PursueSteering();
	mAlignToMovementSteering = new AlignToMovementSteering();
}

void Pursue::onEnter() {	
	mOwner->getCharacter()->AddSteering(mPursueSteering);
	mOwner->getCharacter()->AddSteering(mAlignToMovementSteering);
}

Status Pursue::update(float step)
{
	float sqrdDist = mOwner->getCharacter()->GetEnemyPosition().DistSqrd(mOwner->getCharacter()->GetLoc());
	if (sqrdDist  < 500.0f)
	{
		return eSuccess;
	}
	else
	{
		return eRunning;
	}
}

void Pursue::onExit() {
	mOwner->getCharacter()->RemoveSteering(mPursueSteering);
	mOwner->getCharacter()->RemoveSteering(mAlignToMovementSteering);
}