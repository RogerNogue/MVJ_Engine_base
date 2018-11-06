#pragma once
#include "Module.h"
#include <assimp/scene.h>
#include <vector>
class ModuleModelLoader :
	public Module
{
public:
	ModuleModelLoader();
	~ModuleModelLoader();

	bool            Init();
	update_status   Update();
	bool            CleanUp();
	void loadModel();
	void drawModel();
	struct myMesh {
		unsigned vbo = 0;
		unsigned vio = 0;
		unsigned material = 0;
		unsigned numVertices = 0;
		unsigned numIndices = 0;
	};
	struct myMaterial {
		unsigned texture0 = 0;
	};
	//unsigned* indices;
	//std::vector<unsigned> indices;
	std::vector<myMesh> meshes;
	std::vector<myMaterial> materials;
	//variables
private:
	void GenerateMeshData(const aiMesh* mesh);
	void GenerateMaterialData(const aiMaterial* mat);
	//variables

	unsigned texture0;
	int numV;
	const aiScene* scene;
	unsigned vbo = 0;
	unsigned vio = 0;
};

