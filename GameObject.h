#ifndef __ModuleGameObject_H__
#define __ModuleGameObject_H__

#include "Globals.h"
#include "MathGeoLib/include/Geometry/AABB.h"
#include <vector>
class ComponentMesh;
class ComponentMaterial;
class ComponentCamera;
class ComponentTransform;
class ComponentShape;
class QuadNode;
class JSON_Value;
class GameObject
{
public:
	GameObject(const char* n);
	GameObject(const char* n, GameObject* parent);
	GameObject(const char* n, GameObject* parent, bool physical);
	GameObject(const char* n, GameObject* parent, int signal, bool light);
	GameObject(JSON_Value* objValue);
	~GameObject();
	void deleteObject();
	void deleteChild(unsigned idc);
	void calculateAABB();
	void ChangeName(char* n);
	void activeToggled();
	void staticToggled(bool first);
	void updateQuadTree();
	void saveObject(JSON_Value* objValue);
	inline bool isPhysical() { return Physical; }
	inline bool IsLight() { return light; }
	void DrawProperties();
	void DrawShapeEditor();
	void LoadNextMaterial();
	void DrawMaterialCreator();

	//variables
	unsigned int id = 0u;
	unsigned int parentId = 0u;
	bool active = true;
	bool isStatic = true;
	bool paintBB = false;
	const char* name = nullptr;
	bool hascamera = false;
	bool hasmesh = false;
	bool hasmaterial = false;
	bool hasShape = false;
	ComponentMesh* mesh = nullptr;
	ComponentShape* shape = nullptr;
	std::vector<GameObject*> meshesOrShapes;
	ComponentCamera* camera = nullptr;
	std::vector<ComponentMaterial*> materials;
	ComponentTransform* transform = nullptr;
	std::vector<GameObject*> children;
	GameObject* parent = nullptr;
	float minX, maxX, minY, maxY, minZ, maxZ;//variables for the bounding box
	AABB boundingBox;
	std::vector<QuadNode*> nodesItAppears;

private:
	//functions
	void toggleMeshActivation();
	void DrawComboBoxMaterials(const char* id, ComponentMaterial mat, static std::string& currentTexture);
	//variables
	bool light = false;
	bool Physical = false;
	bool BBGenerated = false;
};
#endif
