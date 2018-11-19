#include "ComponentTransform.h"



ComponentTransform::ComponentTransform(const GameObject* dad)
{
	type = TRANSFORM;
	this->dad = dad;
	active = false;
	positionValues = rotationValues = scaleValues = math::float4x4::identity;
}


ComponentTransform::~ComponentTransform()
{
}

update_status ComponentTransform::Update() {
	transformMatrix = positionValues * rotationValues * scaleValues;
	return UPDATE_CONTINUE;
}

void ComponentTransform::move(math::float3 newPos) {
	positionValues[0][3] = newPos[0]; positionValues[1][3] = newPos[1]; positionValues[2][3] = newPos[2];
}
void ComponentTransform::rotate(math::float3x3 newRot) {
	rotationValues[0][0] = newRot[0][0]; rotationValues[0][1] = newRot[0][1]; rotationValues[0][2] = newRot[0][2];
	rotationValues[1][0] = newRot[1][0]; rotationValues[1][1] = newRot[1][1]; rotationValues[1][2] = newRot[1][2];
	rotationValues[2][0] = newRot[2][0]; rotationValues[2][1] = newRot[2][1]; rotationValues[2][2] = newRot[2][2];
}
void ComponentTransform::scale(math::float3x3 newScale) {
	scaleValues[0][0] = newScale[0][0];
	scaleValues[1][1] = newScale[1][1];
	scaleValues[2][2] = newScale[2][2];
}