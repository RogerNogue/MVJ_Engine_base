#include "ComponentMesh.h"
#include "GL/glew.h"
#include "Application.h"
#include "Serializer.h"

ComponentMesh::ComponentMesh(GameObject* dad):
	Component(dad)
{
	type = MESH;
	active = true;
	id = App->generateID();
}

ComponentMesh::ComponentMesh(JSON_Value* meshFile, GameObject* dad) :
	Component(dad)
{
	type = MESH;
	active = meshFile->getBool("Active");
	id = meshFile->getUint("ID");
	mesh.material = meshFile->getUint("Material");
	mesh.numIndices = meshFile->getUint("Number of indices");
	mesh.numVertices = meshFile->getUint("Number of vertices");
	mesh.vbo = meshFile->getUint("VBO");
	mesh.vio = meshFile->getUint("IBO");

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

void ComponentMesh::saveMesh(JSON_Value* val) {
	JSON_Value* meshval = val->createValue();
	//meshval->convertToArray();

	meshval->addBool("Active", active);
	meshval->addUint("ID", id);
	meshval->addInt("Type", type);
	meshval->addUint("VBO", mesh.vbo);
	meshval->addUint("IBO", mesh.vio);
	meshval->addUint("Material", mesh.material);
	meshval->addUint("Number of vertices", mesh.numVertices);
	meshval->addUint("Number of indices", mesh.numIndices);

	val->addValue("Mesh", meshval);
}