#include <stdafx.h>
#include "hit_action.h"

#include "character.h"
#include "stateMachine.h"

void HitAction::start() {
	mHitTimer = 0.0f;
}

void HitAction::update(float step) {
	mHitTimer += step;
	if (mHitTimer >= 0.5f) {
		mOwner->getCharacter()->SetIsHit(false);
	}
}