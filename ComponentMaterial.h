#ifndef __ComponentMaterial_H__
#define __ComponentMaterial_H__
#include "Component.h"
class ComponentMaterial :
	public Component
{
public:
	ComponentMaterial(GameObject* dad);
	~ComponentMaterial();
	bool CleanUp() override;

	//data structures
	struct myMaterial {
		unsigned texture0 = 0;
	};

	//vars
	myMaterial material;
};
#endif