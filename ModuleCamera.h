#pragma once
#include "Module.h"
#include "MathGeoLib.h"
class ModuleCamera :
	public Module
{
public:
	ModuleCamera();
	~ModuleCamera();

	void lookAt(const math::float3& obs, const math::float3& vrp, const math::float3& up);

	bool            Init();
	update_status   Update();
	bool            CleanUp();

	void setFoV(float fov);//changes vertical Fov
	void SetAspectRatio(float aspect);//changes horizontal Fov
	//variables
	float movementSpeed;

private:
	bool cameraChanged;

};

