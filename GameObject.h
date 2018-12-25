#ifndef __ModuleGameObject_H__
#define __ModuleGameObject_H__

#include "Globals.h"
#include "MathGeoLib/include/Geometry/AABB.h"
#include <vector>
class ComponentMesh;
class ComponentMaterial;
class ComponentCamera;
class ComponentTransform;
class QuadNode;
class JSON_Value;
class GameObject
{
public:
	GameObject(char* n);
	GameObject(char* n, GameObject* parent);
	~GameObject();
	void deleteObject();
	void deleteChild(unsigned idc);
	void createEmptyComponent(component_type type);
	void calculateAABB();
	void ChangeName(char* n);
	void activeToggled();
	void staticToggled(bool first);
	void updateQuadTree();
	void saveObject(JSON_Value* objValue);

	//variables
	component_type type;
	unsigned int id;
	bool active;
	bool isStatic = true;
	bool paintBB = false;
	char* name;
	bool hascamera = false;
	bool hasmesh = false;
	bool hasmaterial = false;
	std::vector<ComponentMesh*> meshes;
	ComponentCamera* camera = nullptr;
	std::vector<ComponentMaterial*> materials;
	ComponentTransform* transform = nullptr;
	std::vector<GameObject*> children;
	GameObject* parent = nullptr;
	float minX, maxX, minY, maxY, minZ, maxZ;//variables for the bounding box
	AABB boundingBox;
	std::vector<QuadNode*> nodesItAppears;

private:
	bool BBGenerated = false;
};
#endif
