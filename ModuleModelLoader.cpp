#include "ModuleModelLoader.h"
#include "Application.h"
#include "ModuleMenu.h"
#include "ModuleProgram.h"
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
	return true;
}
update_status   ModuleModelLoader::Update() {
	/*
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	*/
	return UPDATE_CONTINUE;
}
bool           ModuleModelLoader::CleanUp(){
	return true;
}

void ModuleModelLoader::loadModel() {
	scene = aiImportFile("models/baker_house/BakerHouse.fbx", 0);
	//scene = aiImportFile("models/gnome/mano.fbx", 0);
	//scene = aiImportFile("models/sword/sword_fbx.fbx", 0);
	if (scene == nullptr) {
		App->menu->consoleLog(aiGetErrorString());
		LOG(aiGetErrorString());
	}
}

void ModuleModelLoader::drawModel() {
	for (unsigned i = 0; i < scene->mNumMeshes; ++i)
	{
		GenerateMeshData(scene->mMeshes[i]);
	}
	for (unsigned i = 0; i < scene->mNumMaterials; ++i)
	{
		GenerateMaterialData(scene->mMaterials[i]);
	}
}

void ModuleModelLoader::GenerateMeshData(aiMesh* mesh) {

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, mesh->mNumVertices * 5 * sizeof(float), nullptr, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, mesh->mNumVertices * 3 * sizeof(float), &mesh->mVertices);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * mesh->mNumVertices * 3, mesh->mVertices);
	

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,                  // attribute 0
		3,                  // number of componentes (3 floats)
		GL_FLOAT,           // data type
		GL_FALSE,           // should be normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	glDrawArrays(GL_TRIANGLES, 0, mesh->mNumVertices);
	glDisableVertexAttribArray(0);

	glUseProgram(App->shaderProgram->programGeometry);

	/*
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0,
		(void*)((mesh->mNumVertices) * (sizeof(float) * 3)) // buffer offset
	);*/

	//glUnmapBuffer(GL_ARRAY_BUFFER);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void ModuleModelLoader::GenerateMaterialData(aiMaterial* mat) {

}