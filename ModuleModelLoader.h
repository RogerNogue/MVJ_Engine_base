#pragma once
#include "Module.h"
#include <assimp/scene.h>
class ModuleModelLoader :
	public Module
{
public:
	ModuleModelLoader();
	~ModuleModelLoader();

	void GenerateMeshData(aiMesh* mesh);
	void GenerateMaterialData(aiMaterial* mat);

	bool            Init();
	update_status   Update();
	bool            CleanUp();
	void loadModel();
	void drawModel();

	//variables
private:

	//variables
	int numV;
	const aiScene* scene;
	unsigned vbo = 0;
};

