#pragma once
#include "condition.h"

class EnemySeenCondition : public Condition {
public:
	virtual bool check() const;
};