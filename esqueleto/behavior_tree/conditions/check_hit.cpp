#include <stdafx.h>
#include "check_hit.h"
#include "character.h"
#include "behavior_tree/behavior_tree.h"

Status CheckHit::update(float step) {
	if (mOwner->getCharacter()->GetIsHit()) {
		return eSuccess;
	}
	else {
		return eFail;
	}
}
