#ifndef __ModuleRenderExercise_h__
#define __ModuleRenderExercise_h__

#include "Module.h"
#include "MathGeoLib.h"

class ModuleRenderExercise : public Module
{
public:
    ModuleRenderExercise();
    ~ModuleRenderExercise();

	bool            Init    ();
	update_status   Update  ();
	bool            CleanUp ();
	void reCalculateVRP(math::float3 p);
	//void lookAt(const math::float3);

	//variables
	math::float3 cam;
	math::float3 vrp;
	math::float3 up;
	math::float3 fwd;
	math::float3 side;

	math::float4x4 model;
	math::float4x4 view;
	math::float4x4 projection;

private:
    unsigned vbo        = 0;

};

#endif /* __ModuleRenderExercise_h__ */
