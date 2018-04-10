#include <stdafx.h>
#include "alignToMovementSteering.h"
#include "alignSteering.h"
#include "character.h"

void AlignToMovementSteering::GetSteering(Character& character, USVec2D& linearAcceleration, float& angularAcceleration) {
	USVec2D linearVelocity = character.GetLinearVelocity();
	mAlignSteering.setTargetRotation(atan2(linearVelocity.mY, linearVelocity.mX) * 57.2958f);
	mAlignSteering.GetSteering(character, linearAcceleration, angularAcceleration);
}

void AlignToMovementSteering::DrawDebug()
{


}