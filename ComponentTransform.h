#ifndef __ComponentTransform_H__
#define __ComponentTransform_H__
#include "Component.h"
class ComponentTransform :
	public Component
{
public:
	ComponentTransform(const GameObject* dad);
	~ComponentTransform();
};
#endif

