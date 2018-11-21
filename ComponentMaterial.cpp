#include "ComponentMaterial.h"
#include "Application.h"
#include "ModuleTextures.h"


ComponentMaterial::ComponentMaterial(const GameObject* dad)
{
	type = MATERIAL;
	this->dad = dad;
	active = false;
}


ComponentMaterial::~ComponentMaterial()
{
	App->textures->Unload(material.texture0);
}
