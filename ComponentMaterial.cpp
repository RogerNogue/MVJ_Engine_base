#include "ComponentMaterial.h"



ComponentMaterial::ComponentMaterial(const GameObject* dad)
{
	type = MATERIAL;
	this->dad = dad;
	active = false;
}


ComponentMaterial::~ComponentMaterial()
{
}
