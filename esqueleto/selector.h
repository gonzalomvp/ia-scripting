#pragma once
#include "group.h"

class Selector : public Group
{
protected:
	void onEnter();
	Status update();
	int m_CurrentChild;
};