#ifndef __ComponentShape_H__
#define __ComponentShape_H__

#include "Component.h"
#include "MathGeoLib.h"
class JSON_Value;
class ComponentShape :
	public Component
{
public:
	ComponentShape(GameObject* dad);
	ComponentShape(JSON_Value* shapeFile, GameObject* dad);
	~ComponentShape();
	bool CleanUp()override;
	void generateSphere(unsigned slices, unsigned stacks, math::float4* color);
	void saveShape(JSON_Value* val);

	//variables
	math::float4 color;
	unsigned slices;
	unsigned stacks;
};
#endif
