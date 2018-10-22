#pragma once
#include "behavior.h"
#include <vector>

class Group : public Behavior
{
protected:
	typedef std::vector<Behavior*> Behaviors;
	Behaviors m_Children;
};