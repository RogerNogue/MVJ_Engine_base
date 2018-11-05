#pragma once
#include "Module.h"
#include "Globals.h"
#include "MathGeoLib.h"

struct SDL_Texture;
struct SDL_Renderer;
struct SDL_Rect;

class ModuleRender : public Module
{
public:
	ModuleRender();
	~ModuleRender();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();
	void WindowResized(unsigned width, unsigned height);
	//variables
	math::float4x4 model;
	math::float4x4 view;
	math::float4x4 projection;
	Frustum frustum;
	void* context;
private:
	void drawAxis();
};
