#include "ModuleModelLoader.h"
#include "Application.h"
#include "ModuleMenu.h"
#include "ModuleProgram.h"
#include "ModuleTextures.h"
#include "ModuleCamera.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include "GL/glew.h"
#include <assimp/material.h>
#include <assimp/mesh.h>
#include <assimp/scene.h>
#include "ModuleScene.h"
#include "GameObject.h"

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
	return UPDATE_CONTINUE;
}

void ModuleModelLoader::deleteVBO(unsigned vbo) {
	glDeleteBuffers(1, &vbo);
}

void ModuleModelLoader::deleteVIO(unsigned vio) {
	glDeleteBuffers(1, &vio);
}

void ModuleModelLoader::unloadTexture(unsigned tex0) {
	App->textures->Unload(tex0);
}

bool           ModuleModelLoader::CleanUp(){
		return true;
}

void ModuleModelLoader::loadModel(unsigned model) {
	currentModel = model;
	currentModelTriangleCount = 0;
	char* modelName;
	if (model == 1) {
		scene = aiImportFile("models/baker_house/BakerHouse.fbx", aiProcessPreset_TargetRealtime_MaxQuality);
		modelName = "Baker House";
	}
	else if (model == 2) {
		scene = aiImportFile("models/banana/banana.fbx", aiProcessPreset_TargetRealtime_MaxQuality);
		modelName = "Banana";
	}
	else if (model == 3) {
		scene = aiImportFile("models/shield/Shield.FBX", aiProcessPreset_TargetRealtime_MaxQuality);
		modelName = "Shield";
	}
	if (scene == nullptr) {
		LOG(aiGetErrorString());
	}
	char* b = new char[100];
	sprintf(b, "Loading new model:\n");
	App->menu->console.AddLog(b);
	App->menu->console.AddLog(modelName);
	if (scene->mNumMeshes <= 0) {
		sprintf(b, "The model loaded has no meshes:\n");
		App->menu->console.AddLog(b);
		sprintf(b, "\n\n");
		App->menu->console.AddLog(b);
		delete[] b;
		return;
	}
	sprintf(b, "\n\n");
	App->menu->console.AddLog(b);
	delete[] b;
	//game object creation and fill of its mesh and material components
	GameObject* Obj = new GameObject(modelName, App->scene->baseObject);

	Obj->minX = Obj->maxX = scene->mMeshes[0]->mVertices[0].x;
	Obj->minY = Obj->maxY = scene->mMeshes[0]->mVertices[0].y;
	Obj->minZ = Obj->maxZ = scene->mMeshes[0]->mVertices[0].z;

	for (unsigned i = 0; i < scene->mNumMeshes; ++i)
	{
		GenerateMeshData(scene->mMeshes[i], Obj);
		currentModelTriangleCount += scene->mMeshes[i]->mNumVertices / 3;
	}
	Obj->calculateAABB();
	for (unsigned i = 0; i < scene->mNumMaterials; ++i)
	{
		GenerateMaterialData(scene->mMaterials[i], Obj);
	}
	App->camera->mewModelLoaded();

}

void ModuleModelLoader::unloadModels() {
	CleanUp();
}

void ModuleModelLoader::GenerateMeshData(const aiMesh* mesh, GameObject* Obj) {
	ComponentMesh* newMesh = new ComponentMesh (Obj);
	//vbo
	glGenBuffers(1, &newMesh->mesh.vbo);
	glBindBuffer(GL_ARRAY_BUFFER, newMesh->mesh.vbo);

	glBufferData(GL_ARRAY_BUFFER, (sizeof(float) * 3 + sizeof(float) * 2)*mesh->mNumVertices, nullptr, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, mesh->mNumVertices * 3 * sizeof(float), mesh->mVertices);
	
	//buffer for the faces (vio)
	math::float2* textureCoords = (math::float2*) glMapBufferRange(GL_ARRAY_BUFFER, mesh->mNumVertices * 3 * sizeof(float), mesh->mNumVertices * 2 * sizeof(float), GL_MAP_WRITE_BIT);
	for (unsigned i = 0; i < mesh->mNumVertices; ++i) {
		textureCoords[i] = math::float2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
		//i use this loop to update the bounding box values
		if (Obj->minX > mesh->mVertices[i].x) {
			Obj->minX = mesh->mVertices[i].x;
		}
		else if (Obj->maxX < mesh->mVertices[i].x) {
			Obj->maxX = mesh->mVertices[i].x;
		}
		if (Obj->minY > mesh->mVertices[i].y) {
			Obj->minY = mesh->mVertices[i].y;
		}
		else if (Obj->maxY < mesh->mVertices[i].y) {
			Obj->maxY = mesh->mVertices[i].y;
		}
		if (Obj->minZ > mesh->mVertices[i].z) {
			Obj->minZ = mesh->mVertices[i].z;
		}
		else if (Obj->maxZ < mesh->mVertices[i].z) {
			Obj->maxZ = mesh->mVertices[i].z;
		}
	}
	glUnmapBuffer(GL_ARRAY_BUFFER);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &newMesh->mesh.vio);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, newMesh->mesh.vio);
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

	newMesh->mesh.material = mesh->mMaterialIndex;
	newMesh->mesh.numIndices = mesh->mNumFaces*3;
	newMesh->mesh.numVertices = mesh->mNumVertices;
	Obj->meshes.push_back(newMesh);
	Obj->hasmesh = true;
	allMeshes.push_back(newMesh);
}

void ModuleModelLoader::GenerateMaterialData(const aiMaterial* mat, GameObject* Obj) {
	ComponentMaterial* newMat = new ComponentMaterial(Obj);

	aiString file;
	aiTextureMapping mapping;
	unsigned uvindex = 0;
	if (mat->GetTexture(aiTextureType_DIFFUSE, 0, &file, &mapping, &uvindex) == AI_SUCCESS) {
		newMat->material.texture0 = App->textures->Load(file.data, false);
	}
	if (currentModel == 2) newMat->material.texture0 = App->textures->Load("models/banana/banana.png", false);
	if(currentModel == 3) newMat->material.texture0 = App->textures->Load("models/shield/tex.png", false);
	
	Obj->materials.push_back(newMat);
	Obj->hasmaterial = true;
}

