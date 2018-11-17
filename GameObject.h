#ifndef __ModuleGameObject_H__
#define __ModuleGameObject_H__

#include "Globals.h"
#include <vector>
class Component;

class GameObject
{
public:
	GameObject(char* n);
	GameObject(char* n, const GameObject* parent);
	~GameObject();

	void Update();
	void createComponent(component_type type);
	void createChildObject(char* n);

	//variables
	bool active;
	char* name;
	std::vector<Component*> components;
	std::vector<GameObject*> children;
	const GameObject* parent;
	//booleans to check for components that must not be repeated
	bool hasMaterial, hasTransform, hasCamera;

};
#endif
