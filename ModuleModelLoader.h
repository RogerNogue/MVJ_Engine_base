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
class ComponentShape;
class GameObject;

class ModuleModelLoader :
	public Module
{
public:
	ModuleModelLoader();
	~ModuleModelLoader();

	void loadModel(unsigned model, GameObject* object);
	void unloadModels();
	void deleteVBO(unsigned vbo);
	void deleteVIO(unsigned vio);
	void unloadTexture(unsigned tex0);
	bool loadSphere(GameObject* object);
	bool loadCylinder(GameObject* object);
	bool loadTorus(GameObject* object);
	void GenerateOneMeshData(ComponentMesh* newMesh);
	void GenerateOneMaterialData(ComponentMaterial* newMaterial);
	
	//variables
	unsigned currentModel;
	int currentModelTriangleCount;
	std::vector<ComponentMesh*> allMeshes;
	std::vector<ComponentShape*> allShapes;
	
private:
	//const aiScene* scene;
	void GenerateMeshData(const aiMesh* mesh, GameObject* Obj, int numMesh, int numModel);
	void GenerateMaterialData(const aiMaterial* mat, GameObject* Obj, int model, int i);
	//variables
	
};

#endif