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

	mat->addUint("ID", id);
	mat->addUint("Texture", material.texture0);
	mat->addFloat("Width", material.sizeX);
	mat->addFloat("Height", material.sizeY);

	val->addValue("Material", mat);
}