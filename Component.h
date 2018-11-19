#ifndef __Component_H__
#define __Component_H__

#include "Globals.h"

class GameObject;

class Component
{
public:
	Component() {};

	virtual bool Init() { return true; };
	virtual bool CleanUp(){ return true; };


	virtual update_status Update() { return UPDATE_CONTINUE; };
	virtual void Enable() { active = true; };
	virtual void Disable() { active = false; };

	//variables
	component_type type;
	const GameObject* dad;
	bool active;
};
#endif

//ctrl + coma = navigate to. posar f i escriure nom del file
//ctrl + coma + m, vas a method
//ctrl coma + t, vas a tipus de var
//buscar com fer modo fullscreen, es ctrl + shift + algo
//crear breackpoint "data breakpoint" aleshores comprova una variable en concret i mira qui i quan es canvia
//durant la exec es pot fer un snapshot de la memoria per veure quanta es fa servir en aquell moment
//