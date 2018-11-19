#ifndef __ComponentTransform_H__
#define __ComponentTransform_H__
#include "Component.h"
#include "MathGeoLib.h"
class ComponentTransform :
	public Component
{
public:
	ComponentTransform(const GameObject* dad);
	~ComponentTransform();

	update_status Update() override;

	void move(math::float3 newPos);
	void rotate(math::float3x3 newRot);
	void scale(math::float3x3 newScale);

	//variables
	math::float4x4 positionValues;
	math::float4x4 rotationValues;
	math::float4x4 scaleValues;
	math::float4x4 transformMatrix;

};
#endif

