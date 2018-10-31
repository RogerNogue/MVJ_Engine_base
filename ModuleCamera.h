#pragma once
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
	void camRotationZ(float angle);

	void setFoV(float fov);//changes vertical Fov
	void SetAspectRatio(float aspect);//changes horizontal Fov
	//variables
	float distCamVrp;
	math::float4x4 camTransf;
	math::float3 cam;
	math::float3 vrp;
	math::float3 up;
	math::float3 fwd;
	math::float3 side;
	math::float3 Xaxis;
	math::float3 Yaxis;
	math::float3 Zaxis;
	math::float4x4 camera;
	float movementSpeed;

private:
	void camSetUp();

};

