#include "ComponentTransform.h"
#include "GameObject.h"



ComponentTransform::ComponentTransform(GameObject* dad)
{
	type = TRANSFORM;
	this->dad = dad;
	active = false;
	if (dad->parent != nullptr) {
		positionValues = dad->parent->transform->positionValues;
		rotationValues = dad->parent->transform->rotationValues;
		scaleValues = dad->parent->transform->scaleValues;
	}
	else {
		positionValues = math::float3(0, 0, 0);
		rotationValues = math::float3(0, 0, 0);
		scaleValues = math::float3(1, 1, 1);
	}
	
	transformMatrix = math::float4x4::identity;
	transformMatrix.Set(float4x4::FromTRS(positionValues, Quat::FromEulerXYZ(rotationValues.x, rotationValues.y, rotationValues.z), scaleValues));
}


ComponentTransform::~ComponentTransform()
{
}

update_status ComponentTransform::Update() {
	if (objectMoved) {
		//update all the sub objects
		if (dad->parent != nullptr) {
			//transformMatrix = transformMatrix * dad->parent->transform->transformMatrix;
			float3 temporalRot = float3(rotationValues.x + dad->parent->transform->rotationValues.x,
										rotationValues.y + dad->parent->transform->rotationValues.y,
										rotationValues.z + dad->parent->transform->rotationValues.z);
			transformMatrix.Set(float4x4::FromTRS(
				positionValues + dad->parent->transform->positionValues,
				Quat::FromEulerXYZ(temporalRot.x, temporalRot.y, temporalRot.z),
				scaleValues + dad->parent->transform->scaleValues));
		}
		else {
			transformMatrix.Set(float4x4::FromTRS(positionValues, Quat::FromEulerXYZ(rotationValues.x, rotationValues.y, rotationValues.z), scaleValues));
		}
		dad->calculateAABB();
		for (int i = 0; i < dad->children.size(); ++i) {
			dad->children[i]->transform->objectMoved = true;
			dad->children[i]->transform->Update();
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