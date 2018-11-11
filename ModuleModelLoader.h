#ifndef __ModuleModelLoader_H__
#define __ModuleModelLoader_H__

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
	void loadModel(unsigned model);
	void drawModel();
	void unloadModels();
	//data structures
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
	//variables
	unsigned currentModel;
	std::vector<myMesh> meshes;
	std::vector<myMaterial> materials;
	float minX, maxX, minY, maxY, minZ, maxZ;//variables for the bounding box
	
private:
	void GenerateMeshData(const aiMesh* mesh);
	void GenerateMaterialData(const aiMaterial* mat);
	//variables
	const aiScene* scene;
	unsigned texture0;
	int numV;
	unsigned vbo = 0;
	unsigned vio = 0;
};

#endif