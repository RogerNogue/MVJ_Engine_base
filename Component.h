#ifndef __Component_H__
#define __Component_H__

#include "Globals.h"

class GameObject;

class Component
{
public:
	Component();
	virtual ~Component();

	virtual update_status Update() { return UPDATE_CONTINUE; };
	virtual void Enable() {};
	virtual void Disable() {};

	//variables
	component_type type;
	GameObject* dad;
	bool active;
};
#endif

