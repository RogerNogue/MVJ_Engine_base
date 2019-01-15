#include "ComponentTransform.h"
#include "GameObject.h"
#include "Application.h"
#include "Serializer.h"

ComponentTransform::ComponentTransform(GameObject* dad):
	Component(dad)
{
	type = TRANSFORM;
	active = false;
	id = App->generateID();
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
	if (dad->parent != nullptr) {
		//transformMatrix = transformMatrix * dad->parent->transform->transformMatrix;
		float3 temporalRot = float3(rotationValues.x + dad->parent->transform->rotationValues.x,
			rotationValues.y + dad->parent->transform->rotationValues.y,
			rotationValues.z + dad->parent->transform->rotationValues.z);
		transformMatrix.Set(float4x4::FromTRS(
			positionValues + dad->parent->transform->positionValues,
			Quat::FromEulerXYZ(temporalRot.x, temporalRot.y, temporalRot.z),
			float3 (scaleValues.x * dad->parent->transform->scaleValues.x,
					scaleValues.y * dad->parent->transform->scaleValues.y,
					scaleValues.z * dad->parent->transform->scaleValues.z)));
	}
	else {
		transformMatrix.Set(float4x4::FromTRS(positionValues, Quat::FromEulerXYZ(rotationValues.x, rotationValues.y, rotationValues.z), scaleValues));
	}
}

ComponentTransform::ComponentTransform(JSON_Value* transfFile, GameObject* dad) :
	Component(dad) 
{
	type = TRANSFORM;
	id = transfFile->getUint("ID");
	active = transfFile->getBool("Active");
	objectMoved = false;//may have to switch this, but unlikely
	positionValues = transfFile->getVector3("Position");
	rotationValues = transfFile->getVector3("Rotation");
	scaleValues = transfFile->getVector3("Scale");
	transformMatrix = transfFile->getTransform("Transform matrix");
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
				float3(	scaleValues.x * dad->parent->transform->scaleValues.x,
						scaleValues.y * dad->parent->transform->scaleValues.y,
						scaleValues.z * dad->parent->transform->scaleValues.z)));
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

void ComponentTransform::saveTransform(JSON_Value* val) {
	JSON_Value* transfVal = val->createValue();
	//transfVal->convertToArray();

	transfVal->addUint("ID", id);
	transfVal->addInt("Type", type);
	transfVal->addBool("Active", active);
	transfVal->addBool("object Moved", objectMoved);
	transfVal->addVector3("Position", positionValues);
	transfVal->addVector3("Rotation", rotationValues);
	transfVal->addVector3("Scale", scaleValues);
	transfVal->addTransformMat("Transform matrix", transformMatrix);

	val->addValue("Transform", transfVal);
}

void ComponentTransform::setValues(math::float4x4 newMat) {
	positionValues.x = newMat[0][3]; positionValues.y = newMat[1][3]; positionValues.z = newMat[2][3];
	scaleValues = newMat.ExtractScale();
	float3x3 rotmat = newMat.RotatePart();
	rotationValues = rotmat.ToEulerXYZ();
	objectMoved = true;
	Update();
}