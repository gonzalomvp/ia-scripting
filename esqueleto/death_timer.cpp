#include <stdafx.h>
#include "death_timer.h"

#include "character.h"
#include "stateMachine.h"

void DeathTimer::start() {
	mDeathTimer = 0.0f;
	mOwner->getCharacter()->SetLinearVelocity(0.0f, 0.0f);
	mOwner->getCharacter()->SetAngularVelocity(0.0f);
}

void DeathTimer::update(float step) {
	mDeathTimer += step;
	if (mDeathTimer >= 5.0f) {
		mOwner->getCharacter()->SetLife(5.0f);
		mOwner->getCharacter()->SetLoc(USVec2D(0, 0));
	}
}