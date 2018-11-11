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

	void lookAt();
	void lookAt(const math::float3& obs, const math::float3& vrp, math::float3& up);

	bool            Init();
	update_status   Update();
	bool            CleanUp();
	
	void camTransformation(math::float3 pos);
	void camRotationX(float angle);
	void camRotationY(float angle);
	void camRotationY010(float angle);
	void camRotationZ(float angle);

	void SetAspectRatio(float w, float h);
	//variables
	float aspectRatio;
	float screenWidth;
	float screenHeight;
	float distCamVrp;
	math::float4x4 camTransf;
	math::float3 cam;
	math::float3 vrp;
	math::float3 up;
	math::float3 fwd;
	math::float3 side;
	math::float4x4 camera;
	double timeLastFrame;
	float movementSpeed;
	bool movementOn;
	bool cameraMoved;
	math::float4x4 model;
	math::float4x4 view;
	math::float4x4 projection;
	Frustum frustum;

private:
	
	void camSetUp();

};

#endif