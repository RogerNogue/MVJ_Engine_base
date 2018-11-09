#include "ModuleModelLoader.h"
#include "Application.h"
#include "ModuleMenu.h"
#include "ModuleProgram.h"
#include "ModuleTextures.h"
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include "GL/glew.h"
#include <assimp/material.h>
#include <assimp/mesh.h>
#include "MathGeoLib.h"

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
	{
		for (unsigned i = 0; i < meshes.size(); ++i)
		{
			if (meshes[i].vbo != 0)
			{
				glDeleteBuffers(1, &meshes[i].vbo);
			}

			if (meshes[i].vio != 0)
			{
				glDeleteBuffers(1, &meshes[i].vio);
			}
		}

		for (unsigned i = 0; i < materials.size(); ++i)
		{
			if (materials[i].texture0 != 0)
			{
				App->textures->Unload(materials[i].texture0);
			}
		}
		return true;
	}
}

void ModuleModelLoader::loadModel() {
	scene = aiImportFile("models/baker_house/BakerHouse.fbx", aiProcessPreset_TargetRealtime_MaxQuality);
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

void ModuleModelLoader::GenerateMeshData(const aiMesh* mesh) {
	myMesh newMesh;
	//vbo
	glGenBuffers(1, &newMesh.vbo);
	glBindBuffer(GL_ARRAY_BUFFER, newMesh.vbo);

	glBufferData(GL_ARRAY_BUFFER, (sizeof(float) * 3 + sizeof(float) * 2)*mesh->mNumVertices, nullptr, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, mesh->mNumVertices * 3 * sizeof(float), mesh->mVertices);
	//glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * mesh->mNumVertices * 3, mesh->mVertices);
	
	//buffer for the faces (vio)
	math::float2* textureCoords = (math::float2*) glMapBufferRange(GL_ARRAY_BUFFER, mesh->mNumVertices * 3 * sizeof(float), mesh->mNumVertices * 2 * sizeof(float), GL_MAP_WRITE_BIT);
	for (unsigned i = 0; i < mesh->mNumVertices; ++i) {
		textureCoords[i] = math::float2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
	}
	
	glUnmapBuffer(GL_ARRAY_BUFFER);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &newMesh.vio);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, newMesh.vio);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned)*mesh->mNumFaces * 3, nullptr, GL_STATIC_DRAW);
	unsigned* indices = (unsigned*)glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, 0,
													sizeof(unsigned)*mesh->mNumFaces * 3, GL_MAP_WRITE_BIT);

	for (unsigned i = 0; i < mesh->mNumFaces; ++i)
	{
		assert(mesh->mFaces[i].mNumIndices == 3);

		*(indices++) = mesh->mFaces[i].mIndices[0];
		*(indices++) = mesh->mFaces[i].mIndices[1];
		*(indices++) = mesh->mFaces[i].mIndices[2];
	}

	glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	newMesh.material = mesh->mMaterialIndex;
	newMesh.numIndices = mesh->mNumFaces;
	newMesh.numVertices = mesh->mNumVertices;
	meshes.push_back(newMesh);

}

void ModuleModelLoader::GenerateMaterialData(const aiMaterial* mat) {
	myMaterial newMat;
	aiString file;
	aiTextureMapping mapping;
	unsigned uvindex = 0;
	if (mat->GetTexture(aiTextureType_DIFFUSE, 0, &file, &mapping, &uvindex) == AI_SUCCESS) {
		newMat.texture0 = App->textures->Load(file.data, false);
	}
	
	materials.push_back(newMat);
}