#pragma once

#include "condition.h"

class DistanceCondition : public Condition {
public:
	virtual bool check() const;
};