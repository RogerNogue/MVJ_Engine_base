#ifndef __ModuleGameObject_H__
#define __ModuleGameObject_H__

#include "Globals.h"
#include <vector>
class Component;

class GameObject
{
public:
	GameObject(char* n);
	GameObject(char* n, GameObject* parent);
	~GameObject();

	void createComponent(component_type type);
	void createChildObject(char* n);

	//variables
	unsigned id;
	bool active;
	char* name;
	bool camera;
	std::vector<Component*> components;
	std::vector<GameObject*> children;
	const GameObject* parent;

};
#endif
