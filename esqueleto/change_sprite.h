#pragma once
#include "behavior.h"

class BehaviorTree;

class ChangeSprite : public Behavior
{
public:
	ChangeSprite(BehaviorTree* owner, int imageIndex) : Behavior(owner), mImageIndex(imageIndex) {}

protected:
	virtual void onEnter() override;
	virtual Status update(float step) override;

private:
	int mImageIndex;
};

