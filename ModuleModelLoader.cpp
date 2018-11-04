#include "ModuleModelLoader.h"
#include "Application.h"
#include "ModuleMenu.h"
#include "ModuleRenderExercise.h"
#include "ModuleProgram.h"
#include "ModuleTextures.h"
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
	scene = aiImportFile("models/baker_house/BakerHouse.fbx", 0);
	texture0 = App->textures->Load("models/baker_house/BakerHouse.png", false);
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

	glBufferData(GL_ARRAY_BUFFER, mesh->mNumVertices * 5 * sizeof(float), nullptr, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, mesh->mNumVertices * 3 * sizeof(float), &mesh->mVertices);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * mesh->mNumVertices * 3, mesh->mVertices);
	
	//buffer for the faces (vio)
	math::float2* textureCoords = (math::float2*) glMapBufferRange(GL_ARRAY_BUFFER, mesh->mNumVertices * 3 * sizeof(float), mesh->mNumVertices * 2 * sizeof(float), GL_MAP_WRITE_BIT);
	for (unsigned i = 0; i < mesh->mNumVertices; ++i) {
		textureCoords[i] = math::float2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
	}

	//unsigned* indices = (unsigned*)glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(unsigned)*mesh->mNumFaces * 3, GL_MAP_WRITE_BIT);
	std::vector<unsigned> indices;
	for (unsigned i = 0; i < mesh->mNumFaces; ++i)
	{
		assert(mesh->mFaces[i].mNumIndices == 3);

		indices.push_back(mesh->mFaces[i].mIndices[0]);
		indices.push_back(mesh->mFaces[i].mIndices[1]);
		indices.push_back(mesh->mFaces[i].mIndices[2]);
	}


	glUnmapBuffer(GL_ARRAY_BUFFER);
	glGenBuffers(1, &newMesh.vio);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, newMesh.vio);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned)*mesh->mNumFaces * 3, &indices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		0,                  // attribute 0
		3,                  // number of componentes (3 floats)
		GL_FLOAT,           // data type
		GL_FALSE,           // should be normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	//tex coordinates
	glVertexAttribPointer(
		1,                  // attribute 0
		2,                  // number of componentes (3 floats)
		GL_FLOAT,           // data type
		GL_FALSE,           // should be normalized?
		0,                  // stride
		(void*)(sizeof(float) * mesh->mNumVertices * 3)            // array buffer offset
	);

	

	glUseProgram(App->shaderProgram->programGeometry);

	/*

	glActiveTexture(GL_TEXTURE0);

	glUniform1i(glGetUniformLocation(App->shaderProgram->programTexture, "texture0"), 0);
	
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0,
		(void*)((mesh->mNumVertices) * (sizeof(float) * 3)) // buffer offset
	);*/
	
	glDrawArrays(GL_TRIANGLES, 0, mesh->mNumVertices);

	

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glUseProgram(App->shaderProgram->programTexture);

	glUniformMatrix4fv(glGetUniformLocation(App->shaderProgram->programTexture,
		"model"), 1, GL_TRUE, &App->exercise->model[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(App->shaderProgram->programTexture,
		"view"), 1, GL_TRUE, &App->exercise->view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(App->shaderProgram->programTexture,
		"proj"), 1, GL_TRUE, &App->exercise->projection[0][0]);

	glActiveTexture(GL_TEXTURE0);

	glUniform1i(glGetUniformLocation(App->shaderProgram->programTexture, "texture0"), 0);
	glBindTexture(GL_TEXTURE_2D, texture0);


	//glUnmapBuffer(GL_ARRAY_BUFFER);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//glUseProgram(0);
	newMesh.material = mesh->mMaterialIndex;
	newMesh.numIndices = mesh->mNumFaces;
	newMesh.numVertices = mesh->mNumVertices;
	meshes.push_back(newMesh);

}

void ModuleModelLoader::GenerateMaterialData(const aiMaterial* mat) {
	myMaterial newMat;
	aiString texture("models/baker_house/BakerHouse.png");
	aiTextureMapping mapping;
	unsigned uvindex = 0;
	if (mat->GetTexture(aiTextureType_DIFFUSE, 0, &texture, &mapping, &uvindex)) {
		newMat.texture0 = texture0;
	}
	materials.push_back(newMat);
}