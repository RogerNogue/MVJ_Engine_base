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
#include "pcg_random.hpp"

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
	name(n)
{
	active = camera = false;
	id = pcg32_random_r(&pcg32_random_t());
	ComponentTransform c(this);
	components.push_back(&c);
}

GameObject::GameObject(char* n, const GameObject* parent) :
	name(n),
	parent(parent)
{
	active = camera = false;
	parent = nullptr;
}

GameObject::~GameObject()
{
}

void GameObject::createComponent(component_type type) {
	switch (type) {
		case CAMERA:
		{
			if (!camera) {
				char* b = new char[100];
				sprintf(b, "This object already has a camera and cannot have more than one \n\n");
				App->menu->console.AddLog(b);
				delete[] b;
			}
			else {
				ComponentCamera c(this);
				components.push_back(&c);
				camera = true;
			}
			break;
		}
		case MESH:
		{
			ComponentMesh c = App->renderer->createComponentMesh(this);
			components.push_back(&c);
			break;
		}
		case MATERIAL:
		{
			ComponentMaterial c = App->textures->createComponentMaterial(this);
			components.push_back(&c);
			break;
		}
	}
}

void GameObject::createChildObject(char* n) {
	GameObject child(n, this);
	children.push_back(&child);

}