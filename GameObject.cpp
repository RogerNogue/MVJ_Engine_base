#include "GameObject.h"
#include "ComponentMaterial.h"
#include "Component.h"
#include "ModuleCamera.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ComponentMesh.h"
#include "ComponentTransform.h"
#include "Application.h"
#include "ModuleMenu.h"
#include "ComponentCamera.h"

GameObject::GameObject(char* n) :
	name(n)
{
	active = false;
	hasMaterial = hasTransform = hasCamera = false;
	parent = nullptr;
}

GameObject::GameObject(char* n, const GameObject* parent) :
	name(n),
	parent(parent)
{
	active = false;
	hasMaterial = hasTransform = hasCamera = false;
	parent = nullptr;
}


GameObject::~GameObject()
{
}

void GameObject::Update() {
	for (unsigned i = 0; i < components.size(); ++i) {
		if (components[i]->active) {
			components[i]->Update();
		}
	}
}
void GameObject::createComponent(component_type type) {
	switch (type) {
		case CAMERA:
		{
			if (hasCamera) {
				char* b = new char[100];
				sprintf(b, "This object already has a camera and cannot have more than one \n\n");
				App->menu->console.AddLog(b);
				delete[] b;
			}
			else {
				ComponentCamera c = App->camera->createComponentCamera(this);
				components.push_back(&c);
				hasCamera = true;
			}
			break;
		}
		case MESH:
		{
			ComponentMesh c(this);
			ComponentMesh c = App->renderer->createComponentMesh(this);
			components.push_back(&c);
			break;
		}
		case MATERIAL:
		{
			if (hasMaterial) {
				char* b = new char[100];
				sprintf(b, "This object already has a material and cannot have more than one \n\n");
				App->menu->console.AddLog(b);
				delete[] b;
			}
			else {
				ComponentMaterial c = App->textures->createComponentMaterial(this);
				components.push_back(&c);
				hasMaterial = true;
			}
			break;
		}
		case TRANSFORM:
		{
			if (hasTransform) {
				char* b = new char[100];
				sprintf(b, "This object already has a transformation and cannot have more than one \n\n");
				App->menu->console.AddLog(b);
				delete[] b;
			}
			else {
				ComponentTransform c(this);
				components.push_back(&c);
				hasTransform = true;
			}
			break;
		}
	}
}

void GameObject::createChildObject(char* n) {
	GameObject child(n, this);
	children.push_back(&child);

}