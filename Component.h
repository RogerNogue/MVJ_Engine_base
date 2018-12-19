#ifndef __Component_H__
#define __Component_H__

#include "Globals.h"

class GameObject;

class Component
{
public:
	Component(GameObject* dad) {this->dad = dad};

	virtual bool Init() { return true; };
	virtual bool CleanUp(){ return true; };


	virtual update_status Update() { return UPDATE_CONTINUE; };
	virtual void Enable() { active = true; };
	virtual void Disable() { active = false; };

	//variables
	unsigned int id;
	component_type type;
	GameObject* dad = nullptr;
	bool active;
};
#endif