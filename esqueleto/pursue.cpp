#include <stdafx.h>
#include "pursue.h"
#include "alignToMovementSteering.h"
#include "pursueSteering.h"
#include "character.h"
#include "behavior_tree.h"

Pursue::Pursue(BehaviorTree* owner, float arriveDistance) : Behavior(owner), mArriveDistance(arriveDistance) {
	mPursueSteering = new PursueSteering();
	mAlignToMovementSteering = new AlignToMovementSteering();
}

Pursue::~Pursue() {
	delete mPursueSteering;
	delete mAlignToMovementSteering;
}

void Pursue::onEnter() {	
	mOwner->getCharacter()->AddSteering(mPursueSteering);
	mOwner->getCharacter()->AddSteering(mAlignToMovementSteering);
}

Status Pursue::update(float step)
{
	if (mOwner->getCharacter()->GetIsHit()) {
		return eFail;
	}

	float sqrdDist = mOwner->getCharacter()->GetEnemyPosition().DistSqrd(mOwner->getCharacter()->GetLoc());
	if (sqrdDist  <= (mArriveDistance * mArriveDistance))
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