#ifndef __ComponentCamera_H__
#define __ComponentCamera_H__
#include "Component.h"



class ComponentCamera :
	public Component
{
public:
	ComponentCamera(const GameObject* dad);
	~ComponentCamera();

};

#endif