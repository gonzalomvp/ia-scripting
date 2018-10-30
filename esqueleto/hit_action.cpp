#include <stdafx.h>
#include "hit_action.h"
#include "character.h"
#include "stateMachine.h"

void HitAction::start() {
	mHitTimer = 0.0f;
	mOwner->getCharacter()->SetLinearVelocity(0.0f, 0.0f);
	mOwner->getCharacter()->SetAngularVelocity(0.0f);
}

void HitAction::update(float step) {
	mHitTimer += step;
	if (mHitTimer >= 0.5f) {
		mOwner->getCharacter()->SetIsHit(false);
		mOwner->getCharacter()->SetLife(mOwner->getCharacter()->GetLife() - 1.0f);
	}
}