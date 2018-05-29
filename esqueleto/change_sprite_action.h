#pragma once
#include "action.h"

class ChangeSpriteAction : public Action {
public:
	ChangeSpriteAction(StateMachine* owner, int imageIndex) : Action(owner), m_imageIndex(imageIndex) {}
	virtual void start();

private:
	int m_imageIndex;
};