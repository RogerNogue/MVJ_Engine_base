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

// *Really* minimal PCG32 code / (c) 2014 M.E. O'Neill / pcg-random.org
// Licensed under Apache License 2.0 (NO WARRANTY, etc. see website)

typedef struct { uint64_t state;  uint64_t inc; } pcg32_random_t;

uint32_t pcg32_random_r(pcg32_random_t* rng)
{
	uint64_t oldstate = rng->state;
	// Advance internal state
	rng->state = oldstate * 6364136223846793005ULL + (rng->inc | 1);
	// Calculate output function (XSH RR), uses old state for max ILP
	uint32_t xorshifted = ((oldstate >> 18u) ^ oldstate) >> 27u;
	uint32_t rot = oldstate >> 59u;
	return (xorshifted >> rot) | (xorshifted << ((-rot) & 31));
}

GameObject::GameObject(char* n) :
	name(n),
	type(OBJECT)
{
	active = true;
	hascamera = hasmesh = hasmaterial = false;
	id = pcg32_random_r(&pcg32_random_t());
	ComponentTransform c(this);
	transform = &c;	
	camera = nullptr;
}

GameObject::GameObject(char* n, GameObject* parent) :
	name(n),
	parent(parent),
	type(OBJECT)
{
	active = true;
	parent->children.push_back(this);
	hascamera = hasmesh = hasmaterial = false;
	camera = nullptr;
}

GameObject::~GameObject()
{
}

void GameObject::deleteObject() {
	parent->deleteChild(id);
	for (int i = 0; i < children.size(); ++i) {
		children[i]->deleteObject();
		delete children[i];
	}
	for (int i = 0; i < meshes.size(); ++i) {
		delete meshes[i];
	}
	for (int i = 0; i < materials.size(); ++i) {
		delete materials[i];
	}
	children.erase(children.begin(), children.end());
}

void GameObject::deleteChild(unsigned idc) {
	for (int i = 0; i < children.size(); ++i) {
		if (children[i]->id == idc) {
			children.erase(children.begin()+i);
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
				ComponentCamera c(this);
				camera = &c;
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
	boundingBox = math::AABB(float3(minX, minY, minZ), float3(maxX, maxY, maxZ));
}