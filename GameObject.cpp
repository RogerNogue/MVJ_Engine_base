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
	camera = material = transform = nullptr;
	camera = nullptr;
	parent = nullptr;
}

GameObject::GameObject(char* n, const GameObject* parent) :
	name(n),
	parent(parent)
{
	active = false;
	camera = material = transform = nullptr;
	parent = nullptr;
}


GameObject::~GameObject()
{
}
update_status GameObject::UpdateCameras() {
	for (unsigned i = 0; i < children.size(); ++i) {
		children[i]->UpdateCameras();
	}
	//once all the children have the camera calculated, we calculate ours
	if (camera == nullptr) return UPDATE_CONTINUE;
	return camera->Update();
}
update_status GameObject::UpdateTransforms() {
	for (unsigned i = 0; i < children.size(); ++i) {
		children[i]->UpdateTransforms();
	}
	//once all the children have the transform calculated, we calculate ours
	if (transform == nullptr) return UPDATE_CONTINUE;
	return transform->Update();
}
update_status GameObject::UpdateMeshes() {
	for (unsigned i = 0; i < children.size(); ++i) {
		children[i]->UpdateMeshes();
	}
	//once all the children have the meshes calculated, we calculate ours
	if(meshes.size() == 0) return UPDATE_CONTINUE;
	update_status validity = UPDATE_CONTINUE;
	for (unsigned i = 0; i < meshes.size(); ++i) {
		validity = meshes[i]->Update();
		if (validity != UPDATE_CONTINUE) return validity;
	}
	return validity;
}
update_status GameObject::UpdateMaterials() {
	for (unsigned i = 0; i < children.size(); ++i) {
		children[i]->UpdateMaterials();
	}
	//once all the children have the material calculated, we calculate ours
	if (material == nullptr) return UPDATE_CONTINUE;
	return material->Update();
}
update_status GameObject::Update() {
	update_status validity = UPDATE_CONTINUE;
	validity = UpdateCameras();
	if (validity != UPDATE_CONTINUE) return validity;

	validity = UpdateTransforms();
	if (validity != UPDATE_CONTINUE) return validity;

	validity = UpdateMeshes();
	if (validity != UPDATE_CONTINUE) return validity;

	validity = UpdateMaterials();
	if (validity != UPDATE_CONTINUE) return validity;
}
void GameObject::createComponent(component_type type) {
	switch (type) {
		case CAMERA:
		{
			if (camera != nullptr) {
				char* b = new char[100];
				sprintf(b, "This object already has a camera and cannot have more than one \n\n");
				App->menu->console.AddLog(b);
				delete[] b;
			}
			else {
				ComponentCamera c(this);
				camera = &c;
			}
			break;
		}
		case MESH:
		{
			ComponentMesh c = App->renderer->createComponentMesh(this);
			meshes.push_back(&c);
			break;
		}
		case MATERIAL:
		{
			if (material != nullptr) {
				char* b = new char[100];
				sprintf(b, "This object already has a material and cannot have more than one \n\n");
				App->menu->console.AddLog(b);
				delete[] b;
			}
			else {
				material = &App->textures->createComponentMaterial(this);
			}
			break;
		}
		case TRANSFORM:
		{
			if (transform != nullptr) {
				char* b = new char[100];
				sprintf(b, "This object already has a transformation and cannot have more than one \n\n");
				App->menu->console.AddLog(b);
				delete[] b;
			}
			else {
				ComponentTransform c(this);
				material = &c;
			}
			break;
		}
	}
}

void GameObject::createChildObject(char* n) {
	GameObject child(n, this);
	children.push_back(&child);

}