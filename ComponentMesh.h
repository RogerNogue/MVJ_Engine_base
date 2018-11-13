#ifndef __ComponentMesh_H__
#define __ComponentMesh_H__
#include "Component.h"
class ComponentMesh :
	public Component
{
public:
	ComponentMesh(const GameObject* dad);
	~ComponentMesh();
};
#endif
