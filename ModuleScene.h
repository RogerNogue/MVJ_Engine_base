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
	void createGameObject(char* c);
	void setUpGameObjectMenu();
	void addIntoQuadTree(GameObject* obj);
	//add a save and load function

	//variables
	GameObject* objectSelected = nullptr;
	GameObject* baseObject = nullptr;
	std::vector<GameObject*> allObjects;
	QuadTreeGnoblin* quadTree;
private:
	AABB sceneBoundingBox;
	void paintGameObjectTree(GameObject* go);
};
#endif
