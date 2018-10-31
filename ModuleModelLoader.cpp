#include "ModuleModelLoader.h"
#include "Application.h"
#include "ModuleMenu.h"
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/material.h>
#include <assimp/mesh.h>



ModuleModelLoader::ModuleModelLoader()
{
}


ModuleModelLoader::~ModuleModelLoader()
{
}


bool ModuleModelLoader::Init() {
	const aiScene* scene = aiImportFile("barker_house/BakerHouse", 0);
	if (scene == NULL) App->menu->consoleLog(aiGetErrorString());
	return true;
}
update_status   ModuleModelLoader::Update() {
	return UPDATE_CONTINUE;
}
bool           ModuleModelLoader::CleanUp(){
	return true;
}