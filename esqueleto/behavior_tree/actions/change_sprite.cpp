#include <stdafx.h>
#include "change_sprite.h"
#include "character.h"
#include "behavior_tree/behavior_tree.h"

Status ChangeSprite::update(float step) {
	mOwner->getCharacter()->SetImage(mImageIndex);
	return eSuccess;
}