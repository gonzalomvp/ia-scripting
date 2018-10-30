#include <stdafx.h>
#include "chase.h"
#include "steering_behaviors/alignToMovementSteering.h"
#include "steering_behaviors/pursueSteering.h"
#include "character.h"
#include "behavior_tree/behavior_tree.h"

Chase::Chase(BehaviorTree* owner, float arriveDistance, float lostDistance) : Behavior(owner), mArriveDistance(arriveDistance), mLostDistance(lostDistance) {
	mPursueSteering          = new PursueSteering();
	mAlignToMovementSteering = new AlignToMovementSteering();
}

Chase::~Chase() {
	mOwner->getCharacter()->RemoveSteering(mPursueSteering);
	mOwner->getCharacter()->RemoveSteering(mAlignToMovementSteering);
	delete mPursueSteering;
	delete mAlignToMovementSteering;
}

void Chase::onEnter() {
	mOwner->getCharacter()->AddSteering(mPursueSteering);
	mOwner->getCharacter()->AddSteering(mAlignToMovementSteering);
}

Status Chase::update(float step)
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

void Chase::onExit() {
	mOwner->getCharacter()->RemoveSteering(mPursueSteering);
	mOwner->getCharacter()->RemoveSteering(mAlignToMovementSteering);
}