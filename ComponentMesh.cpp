#include "ComponentMesh.h"
#include "GL/glew.h"



ComponentMesh::ComponentMesh(const GameObject* dad)
{
	type = MESH;
	this->dad = dad;
	active = true;
}


ComponentMesh::~ComponentMesh()
{
	if (mesh.vbo != 0)
	{
		glDeleteBuffers(1, &mesh.vbo);
	}

	if (mesh.vio != 0)
	{
		glDeleteBuffers(1, &mesh.vio);
	}
}

bool ComponentMesh::CleanUp() {
	if (mesh.vbo != 0)
	{
		glDeleteBuffers(1, &mesh.vbo);
	}

	if (mesh.vio != 0)
	{
		glDeleteBuffers(1, &mesh.vio);
	}
	return true;
}