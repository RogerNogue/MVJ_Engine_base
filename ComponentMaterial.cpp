#include "ComponentMaterial.h"
#include "Application.h"
#include "ModuleTextures.h"


ComponentMaterial::ComponentMaterial(GameObject* dad)
{
	type = MATERIAL;
	this->dad = dad;
	active = true;
}


ComponentMaterial::~ComponentMaterial()
{
	App->textures->Unload(material.texture0);
}

bool ComponentMaterial::CleanUp() {
	App->textures->Unload(material.texture0);
	return true;
}