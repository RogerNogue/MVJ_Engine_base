#include "ComponentMaterial.h"
#include "Application.h"
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
	material.texture0 = matFile->getUint("Texture");
	material.sizeX = matFile->getFloat("Width");
	material.sizeY = matFile->getFloat("Height");
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
	mat->addUint("Texture", material.texture0);
	mat->addFloat("Width", material.sizeX);
	mat->addFloat("Height", material.sizeY);

	val->addValue("Material", mat);
}