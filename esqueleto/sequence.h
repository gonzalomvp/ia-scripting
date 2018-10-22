#pragma once
#include "group.h"

class Sequence : public Group
{
protected:
	void onEnter();
	Status update();
	int m_CurrentChild;
};