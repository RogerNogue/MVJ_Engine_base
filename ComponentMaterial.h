#ifndef __ComponentMaterial_H__
#define __ComponentMaterial_H__
#include "Component.h"
class JSON_Value;
class ComponentMaterial :
	public Component
{
public:
	ComponentMaterial(GameObject* dad);
	ComponentMaterial(JSON_Value* matFile, GameObject* dad);
	~ComponentMaterial();
	bool CleanUp() override;
	void saveMaterial(JSON_Value* val);

	//data structures
	struct myMaterial {
		unsigned texture0 = 0;
		float sizeX = 0;
		float sizeY = 0;
	};

	//vars
	myMaterial material;
	int numModel = 0;
	int numMaterial = 0;
};
#endif