#ifndef __ComponentMaterial_H__
#define __ComponentMaterial_H__
#include "Component.h"
#include "MathGeoLib.h"
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
	struct mySurface {
		math::float4 ambientColor = math::float4(0, 0, 0, 1);
		math::float4 difuseColor = math::float4(0, 0, 0, 1);
		math::float4 specularColor = math::float4(0, 0, 0, 1);
		math::float4 emisiveColor = math::float4(0, 0, 0, 1);
		unsigned program = 0;
	};

	//vars
	myMaterial material;
	mySurface surface;
	int numModel = 0;
	int numMaterial = 0;
	bool isTexture = true;
};
#endif