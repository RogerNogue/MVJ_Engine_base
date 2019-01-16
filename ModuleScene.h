#ifndef __ModuleScene_H__
#define __ModuleScene_H__
#include "Module.h"
#include <vector>
#include "MathGeoLib.h"

class GameObject;
class QuadTreeGnoblin;

class ModuleScene :
	public Module
{
public:
	ModuleScene();
	~ModuleScene();

	bool Init()override;
	void createGameObject(const char* c);
	void setUpGameObjectMenu();
	void addIntoQuadTree(GameObject* obj);
	//add a save and load function
	void saveScene(const char* name);
	void loadScene(const char* name);
	//mouse picking function
	void mouseClick(int mouseXi, int mouseYi);

	//variables
	GameObject* objectSelected = nullptr;
	GameObject* baseObject = nullptr;
	std::vector<GameObject*> allObjects;
	std::vector<GameObject*> allLights;
	QuadTreeGnoblin* quadTree;
	bool drawQuadTree = false;
	char* folderPath = "Scenes/";
	math::LineSegment ray = LineSegment(float3(-100, -100, -100), float3(-100, -100, -100));
	bool drawSegment = false;
	float sceneScale = 200;

private:
	int sceneNum = 1;
	AABB sceneBoundingBox;
	void paintGameObjectTree(GameObject* go);
	GameObject* closestToCam(GameObject* go1, GameObject* go2);
};
#endif
