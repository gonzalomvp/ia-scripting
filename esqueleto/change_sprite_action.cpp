#include <stdafx.h>
#include "change_sprite_action.h"

#include "character.h"
#include "stateMachine.h"

void ChangeSpriteAction::start() {
	m_owner->getCharacter()->SetImage(m_imageIndex);
}