#ifndef __ModuleModelLoader_H__
#define __ModuleModelLoader_H__

#include "Module.h"
#include <vector>
//#include "MathGeoLib.h"

class aiScene;
class aiMesh;
class aiMaterial;
class ComponentMesh;
class ComponentMaterial;
class GameObject;

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
	void unloadModels();
	void deleteVBO(unsigned vbo);
	void deleteVIO(unsigned vio);
	void unloadTexture(unsigned tex0);
	
	//variables
	unsigned currentModel;
	int currentModelTriangleCount;
	
private:
	const aiScene* scene;
	void GenerateMeshData(const aiMesh* mesh, GameObject* Obj);
	void GenerateMaterialData(const aiMaterial* mat, GameObject* Obj);
	//variables
	
};

#endif