#ifndef __ModuleScene_H__
#define __ModuleScene_H__
#include "Module.h"
#include <vector>

class GameObject;

class ModuleScene :
	public Module
{
public:
	ModuleScene();
	~ModuleScene();

	bool Init()override;
	void createGameObject(char* c);
	void setUpGameObjectMenu();
	//add a save and load function

	//variables
	GameObject* objectSelected = nullptr;
	GameObject* baseObject = nullptr;
	std::vector<GameObject*> objects;
private:
	void paintGameObjectTree(GameObject* go);
};
#endif
