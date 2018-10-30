#include <stdafx.h>
#include "check_dead.h"
#include "character.h"
#include "behavior_tree/behavior_tree.h"

Status CheckDead::update(float step) {
	if (mOwner->getCharacter()->GetLife() <= 0.0f) {
		return eSuccess;
	}
	else {
		return eFail;
	}
}
