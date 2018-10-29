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
	void drawAxis();
	void addTexture();
	void activateTextures(char* name);
	//void lookAt(const math::float3);

	//variables
	math::float4x4 model;
	math::float4x4 view;
	math::float4x4 projection;
	char* activeTexture = "Lenna.png";
	unsigned texture0;
	bool texture;

private:
    unsigned vbo        = 0;

};

#endif /* __ModuleRenderExercise_h__ */
