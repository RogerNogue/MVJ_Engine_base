#include "ComponentCamera.h"



ComponentCamera::ComponentCamera(const GameObject* dad){
	type = CAMERA;
	this->dad = dad;
	active = false;
}


ComponentCamera::~ComponentCamera()
{
	delete this->dad;
}