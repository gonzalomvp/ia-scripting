#include <stdafx.h>
#include "alignToMovementSteering.h"
#include "alignSteering.h"
#include "character.h"

void AlignToMovementSteering::GetSteering(Character& character, USVec2D& linearAcceleration, float& angularAcceleration) {
	USVec2D linearVelocity = character.GetLinearVelocity();
	character.GetParams().targetRotation = RAD2DEG(atan2(linearVelocity.mY, linearVelocity.mX));
	mAlignSteering.GetSteering(character, linearAcceleration, angularAcceleration);
}
