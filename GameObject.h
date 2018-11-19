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

	update_status UpdateCameras();
	update_status UpdateTransforms();
	update_status UpdateMeshes();
	update_status UpdateMaterials();
	//only the root game object will call this update
	update_status Update();
	void createComponent(component_type type);
	void createChildObject(char* n);

	//variables
	bool active;
	char* name;
	std::vector<Component*> meshes;
	Component* camera;
	Component* transform;
	Component* material;
	std::vector<GameObject*> children;
	const GameObject* parent;

};
#endif
