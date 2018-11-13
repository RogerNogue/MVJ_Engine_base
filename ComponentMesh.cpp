#include "ComponentMesh.h"



ComponentMesh::ComponentMesh(const GameObject* dad)
{
	type = MESH;
	this->dad = dad;
	active = false;
}


ComponentMesh::~ComponentMesh()
{
}
