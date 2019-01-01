#include "ComponentMaterial.h"
#include "Application.h"
#include "ModuleModelLoader.h"
#include "ModuleTextures.h"
#include "Application.h"
#include "Serializer.h"

ComponentMaterial::ComponentMaterial(GameObject* dad):
	Component(dad)
{
	type = MATERIAL;
	active = true;
	id = App->generateID();
}

ComponentMaterial::ComponentMaterial(JSON_Value* matFile, GameObject* dad) :
	Component(dad) {
	type = MATERIAL;
	active = matFile->getBool("Active");
	id = matFile->getUint("ID");
	numModel = matFile->getInt("numModel");
	numMaterial = matFile->getInt("numMaterial");

	App->modelLoader->GenerateOneMaterialData(this);
}

ComponentMaterial::~ComponentMaterial()
{
	App->textures->Unload(material.texture0);
}

bool ComponentMaterial::CleanUp() {
	App->textures->Unload(material.texture0);
	return true;
}

void ComponentMaterial::saveMaterial(JSON_Value* val) {
	JSON_Value* mat = val->createValue();
	//mat->convertToArray();

	mat->addUint("ID", id);
	mat->addInt("Type", type);
	mat->addBool("Active", active);
	mat->addInt("numModel", numModel);
	mat->addInt("numMaterial", numMaterial);
	
	val->addValue("Material", mat);
}