#pragma once
#include "state_machine/action.h"

class ChangeSpriteAction : public Action {
public:
	ChangeSpriteAction(StateMachine* owner, int imageIndex) : Action(owner), mImageIndex(imageIndex) {}
	virtual void start() override;

private:
	int mImageIndex;
};