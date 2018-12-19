#include "ComponentMaterial.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "Application.h"

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