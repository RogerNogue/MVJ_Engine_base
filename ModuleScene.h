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
	update_status PreUpdate() override;
	update_status Update() override;
	void createGameObject(char* c);
	void setUpGameObjectMenu();
	//add a save and load function

	//variables
	GameObject* baseObject;
	std::vector<GameObject*> objects;
private:
	void paintGameObjectTree(const GameObject* go);
};
#endif
