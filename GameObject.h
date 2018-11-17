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
	//booleans to check for components that must not be repeated
	bool hasMaterial, hasTransform, hasCamera;

};
#endif
