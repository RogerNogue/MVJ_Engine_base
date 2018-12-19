#include "ComponentMesh.h"
#include "GL/glew.h"
#include "Application.h"


ComponentMesh::ComponentMesh(GameObject* dad):
	Component(dad)
{
	type = MESH;
	active = true;
	id = App->generateID();
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