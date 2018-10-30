#include <stdafx.h>
#include "change_sprite_action.h"
#include "character.h"
#include "state_machine/state_machine.h"

void ChangeSpriteAction::start() {
	mOwner->getCharacter()->SetImage(mImageIndex);
}