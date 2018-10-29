#pragma once
#include "action.h"

class ChangeSpriteAction : public Action {
public:
	ChangeSpriteAction(StateMachine* owner, int imageIndex) : Action(owner), mImageIndex(imageIndex) {}
	virtual void start();

private:
	int mImageIndex;
};