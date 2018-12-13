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
#include "ModuleModelLoader.h"
#include "MathGeoLib.h"
#include "ModuleScene.h"
#include "debugdraw.h"
#include "ModuleDebugDraw.h"

GameObject::GameObject(char* n) :
	name(n),
	type(OBJECT)
{
	active = true;
	hascamera = hasmesh = hasmaterial = false;
	id = App->generateID();
	transform = new ComponentTransform(this);	
	camera = nullptr;
	minX = minY = minZ = -1;
	maxX = maxY = maxZ = 1;
	App->scene->allObjects.push_back(this);
}

GameObject::GameObject(char* n, GameObject* parent) :
	name(n),
	parent(parent),
	type(OBJECT)
{
	active = true;
	parent->children.push_back(this);
	hascamera = hasmesh = hasmaterial = false;
	id = App->generateID();
	transform = new ComponentTransform(this);
	camera = nullptr;
	minX = minY = minZ = -1;
	maxX = maxY = maxZ = 1;
	App->scene->allObjects.push_back(this);
}

GameObject::~GameObject()
{
}

void GameObject::deleteObject() {
	char* b = new char[100];
	sprintf(b, "Deleting object: ");
	App->menu->console.AddLog(b);
	App->menu->console.AddLog(name);
	sprintf(b, "\n\n");
	App->menu->console.AddLog(b);
	delete[] b;

	for (int i = 0; i < children.size(); ++i) {
		children[i]->deleteObject();
		delete children[i];
	}
	parent->deleteChild(id);
	children.clear();
	for (int i = 0; i < meshes.size(); ++i) {
		for (int j = 0; j < App->modelLoader->allMeshes.size(); ++j) {
			if (App->modelLoader->allMeshes[j] == meshes[i]) {
				//delete App->modelLoader->allMeshes[j];
				App->modelLoader->allMeshes.erase(App->modelLoader->allMeshes.begin()+j);
			}
		}
		meshes[i]->CleanUp();
		delete meshes[i];
	}
	meshes.clear();
	for (int i = 0; i < materials.size(); ++i) {
		materials[i]->CleanUp();
		delete materials[i];
	}
	materials.clear();
}

void GameObject::deleteChild(unsigned idc) {
	for (int i = 0; i < children.size(); ++i) {
		if (children[i]->id == idc) {
			children.erase(children.begin()+i);
			return;
		}
	}
}

void GameObject::createEmptyComponent(component_type type) {
	switch (type) {
		case CAMERA:
		{
			if (!hascamera) {
				char* b = new char[100];
				sprintf(b, "This object already has a camera and cannot have more than one \n\n");
				App->menu->console.AddLog(b);
				delete[] b;
			}
			else {
				camera = new ComponentCamera(this);
				hascamera = true;
			}
			break;
		}
		case MESH:
		{
			hasmesh = true;
			ComponentMesh c = App->renderer->createComponentMesh(this);
			meshes.push_back(&c);
			break;
		}
		case MATERIAL:
		{
			hasmaterial = true;
			ComponentMaterial c = App->textures->createComponentMaterial(this);
			materials.push_back(&c);
			break;
		}
	}
}

void GameObject::calculateAABB() {
	//boundingBox.Transform(transform->transformMatrix);
	boundingBox.TransformAsAABB(transform->transformMatrix);
	if (!BBGenerated) {
		boundingBox = math::AABB(float3(minX, minY, minZ), float3(maxX, maxY, maxZ));
		BBGenerated = true;
	}
}

void GameObject::ChangeName(char* n) {
	name = n;
}

void GameObject::activeToggled() {
	for (int i = 0; i < meshes.size(); ++i) {
		meshes[i]->active = active;
	}
	for (int i = 0; i < materials.size(); ++i) {
		materials[i]->active = active;
	}
	if (hascamera) {
		camera->active = active;
	}
	transform->active = active;
}

void GameObject::staticToggled(bool first) {
	if(!first)isStatic = !isStatic;
	for (int i = 0; i < children.size(); ++i) {
		children[i]->staticToggled(false);
	}
}