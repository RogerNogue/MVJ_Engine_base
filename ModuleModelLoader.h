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
struct par_shapes_mesh;

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
	bool CreateSphere(GameObject* object);
	bool CreateCube(GameObject* object);
	bool CreateCylinder(GameObject* object);
	bool CreateTorus(GameObject* object);

	bool LoadSphere(ComponentShape* sphere);
	bool LoadCube(ComponentShape* cube);
	bool LoadCylinder(ComponentShape* cylinder);
	bool LoadTorus(ComponentShape* torus);

	void GenerateOneMeshData(ComponentMesh* newMesh);
	void GenerateOneMaterialData(ComponentMaterial* newMaterial);
	
	//variables
	unsigned currentModel = 0;
	int currentModelTriangleCount = 0;
	std::vector<ComponentMesh*> allMeshes;
	std::vector<ComponentShape*> allShapes;
	
private:
	//const aiScene* scene;
	void GenerateMeshData(const aiMesh* mesh, GameObject* Obj, int numMesh, int numModel);
	void GenerateMaterialData(const aiMaterial* mat, GameObject* Obj, int model, int i);
	void generateShape(par_shapes_mesh* shape, ComponentShape* comp, ComponentMaterial* mat);
	//variables
	
};

#endif