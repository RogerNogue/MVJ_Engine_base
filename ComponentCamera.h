#ifndef __ComponentCamera_H__
#define __ComponentCamera_H__
#include "Component.h"
#include "MathGeoLib.h"

//class for the cameras that are not the editor camera
class JSON_Value;
class ComponentCamera :
	public Component
{
public:
	ComponentCamera(GameObject* dad);
	~ComponentCamera();

	bool Init() override;
	update_status Update() override;
	bool CleanUp() override;

	void camRotationX(float angle);
	void camRotationY(float angle);
	void updateCam();
	void SetAspectRatio(float w, float h);
	void mewModelLoaded();
	ComponentCamera createComponentCamera(GameObject* dad);
	void saveCamera(JSON_Value* val);

	//variables
	float aspectRatio;
	float screenWidth;
	float screenHeight;
	float distCamVrp;
	math::float3 vrp;
	math::float4x4 camera;
	float movementSpeed;
	float mouseRotSpeed;
	math::float4x4 model;
	math::float4x4 view;
	math::float4x4 projection;
	Frustum frustum;
	float3 modelCenter;
	float modelWidth, modelHeight;
	bool cameraChanged;

};

#endif