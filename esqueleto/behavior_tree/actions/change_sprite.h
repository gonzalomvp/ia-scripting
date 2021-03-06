#pragma once
#include "behavior_tree/behavior.h"

class BehaviorTree;

class ChangeSprite : public Behavior {
public:
	ChangeSprite(BehaviorTree* owner, int imageIndex) : Behavior(owner), mImageIndex(imageIndex) {}

protected:
	virtual Status update(float step) override;

private:
	int mImageIndex;
};

