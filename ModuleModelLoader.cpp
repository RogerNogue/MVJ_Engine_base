#include "ModuleModelLoader.h"
#include "Application.h"
#include "ModuleMenu.h"
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include "GL/glew.h"
#include <assimp/material.h>
#include <assimp/mesh.h>

ModuleModelLoader::ModuleModelLoader()
{
}


ModuleModelLoader::~ModuleModelLoader()
{
}


bool ModuleModelLoader::Init() {
	scene = aiImportFile("barker_house/BakerHouse.fbx", 0);
	if (scene == nullptr) {
		App->menu->consoleLog(aiGetErrorString());
		LOG(aiGetErrorString());
	}
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	for (unsigned i = 0; i < scene->mNumMeshes; ++i)
	{
		GenerateMeshData(scene->mMeshes[i]);
	}
	for (unsigned i = 0; i < scene->mNumMaterials; ++i)
	{
		GenerateMaterialData(scene->mMaterials[i]);
	}
	return true;
}
update_status   ModuleModelLoader::Update() {
	return UPDATE_CONTINUE;
}
bool           ModuleModelLoader::CleanUp(){
	return true;
}

void ModuleModelLoader::GenerateMeshData(aiMesh* mesh) {

	
	/*glBufferData(GL_ARRAY_BUFFER, mesh->mNumVertices * 5 * sizeof(unsigned int), nullptr, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, mesh->mNumVertices * 3 * sizeof(unsigned int), &mesh->mVertices);
	glBufferSubData(GL_ARRAY_BUFFER, mesh->mNumVertices * 3 * sizeof(unsigned int), mesh->mNumVertices * 2 * sizeof(unsigned int), &mesh->mTextureCoords);
	*/
	//potser la mida es 5*3*sizeof
	glBufferData(GL_ARRAY_BUFFER, mesh->mNumVertices * 3 * sizeof(unsigned int), nullptr, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, mesh->mNumVertices * 3 * sizeof(unsigned int), &mesh->mVertices);

}

void ModuleModelLoader::GenerateMaterialData(aiMaterial* mat) {

}