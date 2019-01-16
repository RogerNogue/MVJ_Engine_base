#include "ComponentMesh.h"
#include "GL/glew.h"
#include "Application.h"
#include "Serializer.h"
#include "ModuleModelLoader.h"
#include "GameObject.h"

ComponentMesh::ComponentMesh(GameObject* dad):
	Component(dad)
{
	dad->mesh = this;
	dad->hasmesh = true;
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
	numMesh = meshFile->getUint("numMesh");
	numModel = meshFile->getUint("numModel");
	path = meshFile->getString("path");
	mesh.material = meshFile->getUint("material");
	
	App->modelLoader->GenerateOneMeshData(this);
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
	meshval->addUint("numModel", numModel);
	meshval->addUint("numMesh", numMesh);
	meshval->addUint("material", mesh.material);
	meshval->addString("path", path);

	val->addValue("Mesh", meshval);
}

void ComponentMesh::setPath(const char* path) {
	char* copy = new char[strlen(path)];
	strcpy(copy, path);
	this->path = copy;
}