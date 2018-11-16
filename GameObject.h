#ifndef __ModuleGameObject_H__
#define __ModuleGameObject_H__

#include "Globals.h"
#include <vector>
class Component;

class GameObject
{
public:
	GameObject(char* n);
	~GameObject();

	void Update();
	void createComponent(component_type type);

	//variables
	bool active;
	char* name;
	std::vector<Component*> components;

};
#endif
