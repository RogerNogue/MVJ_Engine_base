#ifndef __ModuleScene_H__
#define __ModuleScene_H__
#include "Module.h"
#include "Globals.h"
#include <vector>

class GameObject;

class ModuleScene :
	public Module
{
public:
	ModuleScene();
	~ModuleScene();

	void createGameObject(char* c);

	//variables
	std::vector<GameObject*> objects;
};
#endif
