#include "change_sprite_action.h"

#include "character.h"
#include "stateMachine.h"

ChangeSpriteAction::ChangeSpriteAction(const char* imageFile) {
	strcpy_s(m_imageFile, imageFile);
}

void ChangeSpriteAction::start() {
	m_owner->getOwner()->SetImage();
}