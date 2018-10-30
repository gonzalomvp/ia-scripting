#include <stdafx.h>
#include "sequence.h"

Status Sequence::update(float step)
{
	while (true)
	{
		Status s = mChildren[mCurrentChild]->tick(step);
		if (s != eSuccess)
		{
			return s;
		}
		++mCurrentChild;

		if (mCurrentChild == mChildren.size())
		{
			return eSuccess;
		}
	}
	return eInvalid;
}

void Sequence::onEnter()
{
	mCurrentChild = 0;
}