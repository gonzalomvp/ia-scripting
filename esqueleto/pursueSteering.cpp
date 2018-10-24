#include <stdafx.h>
#include "pursueSteering.h"
#include "arriveSteering.h"
#include "character.h"

void PursueSteering::GetSteering(Character& character, USVec2D& linearAcceleration, float& angularAcceleration)
{
	float maxVelocity = character.GetParams().max_velocity;
	USVec2D enemyPos = character.GetEnemyPosition();
	float distance = enemyPos.Dist(character.GetLoc());
	float timeToReachEnemy = distance / maxVelocity;
	enemyPos += character.GetEnemyVelocity() * timeToReachEnemy;

	character.GetParams().targetPosition = enemyPos;
	mArriveSteering.GetSteering(character, linearAcceleration, angularAcceleration);
}

void PursueSteering::DrawDebug()
{
	mArriveSteering.DrawDebug();

}