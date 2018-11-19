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

	update_status PreUpdate() override;
	update_status Update() override;
	void createGameObject(char* c);
	//add a save and load function

	//variables
	std::vector<GameObject*> objects;
};
#endif
