#ifndef __ModuleCamera_H__
#define __ModuleCamera_H__

#include "Module.h"
#include "MathGeoLib.h"

class ModuleCamera :
	public Module
{
public:
	ModuleCamera();
	~ModuleCamera();

	bool            Init();
	update_status   Update();
	bool            CleanUp();
	
	void camRotationX(float angle);
	void camRotationY(float angle);

	void SetAspectRatio(float w, float h);
	void mewModelLoaded();
	//variables
	float aspectRatio;
	float screenWidth;
	float screenHeight;
	float distCamVrp;
	math::float3 vrp;
	math::float4x4 camera;
	double timeLastFrame;
	float movementSpeed;
	float mouseRotSpeed;
	bool movementOn;
	bool cameraMoved;
	math::float4x4 model;
	math::float4x4 view;
	math::float4x4 projection;
	Frustum frustum;
	float3 modelCenter;
private:

};

#endif