#include <stdafx.h>
#include "pursueSteering.h"
#include "arriveSteering.h"
#include "character.h"

void PursueSteering::GetSteering(Character& character, USVec2D& linearAcceleration, float& angularAcceleration)
{
	USVec2D enemyPos = character.GetEnemyPosition();
	float distance = enemyPos.Dist(character.GetLoc());
	float timeToReachEnemy = distance / mMaxVelocity;
	enemyPos.mX += character.GetEnemySpeed() * timeToReachEnemy;

	mArriveSteering.setTargetPosition(enemyPos);
	mArriveSteering.GetSteering(character, linearAcceleration, angularAcceleration);
}

void PursueSteering::DrawDebug()
{
	mArriveSteering.DrawDebug();

}