#ifndef __ComponentTransform_H__
#define __ComponentTransform_H__
#include "Component.h"
#include "MathGeoLib.h"
class ComponentTransform :
	public Component
{
public:
	ComponentTransform(GameObject* dad);
	~ComponentTransform();
	update_status Update() override;
	void placeAt000();

	//variables
	bool objectMoved = false;
	math::float3 positionValues = math::float3(0, 0, 0);
	math::float3 rotationValues = math::float3(0, 0, 0);
	math::float3 scaleValues = math::float3 (1,1,1);
	math::float4x4 transformMatrix = math::float4x4::identity;

};
#endif

