#ifndef __ModuleRender_H__
#define __ModuleRender_H__

#include "Module.h"
#include "MathGeoLib.h"

class ComponentMesh;
class GameObject;

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
	ComponentMesh createComponentMesh(GameObject* dad);
	//variables
	void* context;
private:
	void drawAxis();
};

#endif