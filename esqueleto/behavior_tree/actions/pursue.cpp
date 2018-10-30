#include <stdafx.h>
#include "pursue.h"
#include "steering_behaviors/alignToMovementSteering.h"
#include "steering_behaviors/pursueSteering.h"
#include "character.h"
#include "behavior_tree/behavior_tree.h"

Pursue::Pursue(BehaviorTree* owner, float arriveDistance, float lostDistance) : Behavior(owner), mArriveDistance(arriveDistance), mLostDistance(lostDistance) {
	mPursueSteering = new PursueSteering();
	mAlignToMovementSteering = new AlignToMovementSteering();
}

Pursue::~Pursue() {
	mOwner->getCharacter()->RemoveSteering(mPursueSteering);
	mOwner->getCharacter()->RemoveSteering(mAlignToMovementSteering);
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
	if (sqrdDist > (mLostDistance * mLostDistance)) {
		return eFail;
	}
	if (sqrdDist  <= (mArriveDistance * mArriveDistance)) {
		return eSuccess;
	}
	return eRunning;
}

void Pursue::onExit() {
	mOwner->getCharacter()->RemoveSteering(mPursueSteering);
	mOwner->getCharacter()->RemoveSteering(mAlignToMovementSteering);
}