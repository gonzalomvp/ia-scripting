#include <stdafx.h>
#include "change_sprite.h"
#include "character.h"
#include "behavior_tree/behavior_tree.h"

void ChangeSprite::onEnter() {
	mOwner->getCharacter()->SetImage(mImageIndex);
}

Status ChangeSprite::update(float step) {
	return eSuccess;
}