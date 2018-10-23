#include <stdafx.h>
#include "pursue.h"
#include "pursueSteering.h"
#include "character.h"
#include "behavior_tree.h"

Pursue::Pursue(BehaviorTree* owner) : Behavior(owner)
{
	mPursueSteering = new PursueSteering(m_owner->getCharacter()->GetParams());
}

void Pursue::onEnter()
{	
	//m_owner->getCharacter()->AddSteering(mPursueSteering);
	m_owner->getCharacter()->SetImage(3);
}

Status Pursue::update(float step)
{
	USVec2D linearAcceleration(0.0f, 0.0f);
	float angularAcceleration = 0.0f;
	mPursueSteering->GetSteering(*(m_owner->getCharacter()), linearAcceleration, angularAcceleration);

	USVec2D linearVelocity = m_owner->getCharacter()->GetLinearVelocity() + linearAcceleration * step;
	m_owner->getCharacter()->SetLinearVelocity(linearVelocity.mX, linearVelocity.mY);
	return eSuccess;

	float sqrdDist = m_owner->getCharacter()->GetEnemyPosition().DistSqrd(m_owner->getCharacter()->GetLoc());
	if (sqrdDist  < 500.0f)
	{
		return eSuccess;
	}
	else
	{
		return eRunning;
	}
}
