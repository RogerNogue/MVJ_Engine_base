#include "ComponentTransform.h"



ComponentTransform::ComponentTransform(const GameObject* dad)
{
	type = TRANSFORM;
	this->dad = dad;
	active = false;
}


ComponentTransform::~ComponentTransform()
{
}
