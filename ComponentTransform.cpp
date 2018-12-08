#include "ComponentTransform.h"
#include "GameObject.h"



ComponentTransform::ComponentTransform(GameObject* dad)
{
	type = TRANSFORM;
	this->dad = dad;
	active = false;
	positionValues = math::float3(0, 0, 0);
	rotationValues = math::float3(0, 0, 0);
	scaleValues = math::float3(1, 1, 1);
	transformMatrix = math::float4x4::identity;
	transformMatrix.Set(float4x4::FromTRS(positionValues, Quat::FromEulerXYZ(rotationValues.x, rotationValues.y, rotationValues.z), scaleValues));
}


ComponentTransform::~ComponentTransform()
{
}

update_status ComponentTransform::Update() {
	if (objectMoved) {
		transformMatrix.Set(float4x4::FromTRS(positionValues, Quat::FromEulerXYZ(rotationValues.x, rotationValues.y, rotationValues.z), scaleValues));
		dad->calculateAABB();
		//update all the sub objects
		if (dad != nullptr) {
			transformMatrix = transformMatrix * dad->transform->transformMatrix;
			for (int i = 0; i < dad->children.size(); ++i) {
				dad->children[i]->transform->Update();
			}
		}
		objectMoved = false;
	}
	return UPDATE_CONTINUE;
}


void ComponentTransform::placeAt000() {
	positionValues = math::float3(0, 0, 0);
	rotationValues = math::float3(0, 0, 0);
	scaleValues = math::float3(1, 1, 1);
	objectMoved = true;
	if (dad != nullptr) {
		for (int i = 0; i < dad->children.size(); ++i) {
			dad->children[i]->transform->placeAt000();
		}
	}
	Update();
}