#pragma once
#include "condition.h"

class EnemyCloseCondition : public Condition
{
public:
	virtual bool check() const;
};