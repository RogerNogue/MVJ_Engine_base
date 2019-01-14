#ifndef __ModuleCamera_H__
#define __ModuleCamera_H__

#include "Module.h"
#include "MathGeoLib.h"

class ComponentCamera;
class GameObject;

class ModuleCamera :
	public Module
{
public:
	ModuleCamera();
	~ModuleCamera();

	bool            Init() override;
	update_status   Update() override;
	
	void camRotationX(float angle);
	void camRotationY(float angle);
	void updateCam();
	void SetAspectRatio(float w, float h);
	void mewModelLoaded();
	ComponentCamera createComponentCamera(GameObject* dad);

	//variables
	float aspectRatio;
	float screenWidth;
	float screenHeight;
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
	float znear = 1.0f;
	float zfar = 100.0f;

private:
	//variables
	float distCamVrp;
	math::float3 vrp;
};

#endif