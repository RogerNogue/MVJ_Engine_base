#include "GameObject.h"
#include "ComponentMaterial.h"
#include "ComponentCamera.h"
#include "ComponentMesh.h"
#include "ComponentTransform.h"


GameObject::GameObject(char* n)
{
	active = false;
	name = n;
}


GameObject::~GameObject()
{
}

void GameObject::Update() {
	for (int i = 0; i < components.size(); ++i) {
		if (components[i]->active) {
			components[i]->Update();
		}
	}
}
void GameObject::createComponent(component_type type) {
	switch (type) {
		case CAMERA:
		{
			ComponentCamera c(this);
			components.push_back(&c);
			break;
		}
		case MESH:
		{
			ComponentMesh c(this);
			components.push_back(&c);
			break;
		}
		case MATERIAL:
		{
			ComponentMaterial c(this);
			components.push_back(&c);
			break;
		}
		case TRANSFORM:
		{
			ComponentTransform c(this);
			components.push_back(&c);
			break;
		}
	}
}