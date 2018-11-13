#ifndef __ComponentMaterial_H__
#define __ComponentMaterial_H__
#include "Component.h"
class ComponentMaterial :
	public Component
{
public:
	ComponentMaterial(const GameObject* dad);
	~ComponentMaterial();

};
#endif