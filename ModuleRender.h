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
	update_status PreUpdate() override;
	//update_status Update()override;
	void Draw();
	update_status PostUpdate()override;
	bool CleanUp();
	void WindowResized(unsigned width, unsigned height);
	ComponentMesh createComponentMesh(GameObject* dad);
	void UpdateEditorCamera();
	//variables
	void* context;
	unsigned editorCameraBuffer;
	unsigned editorTexture;
	unsigned frameBuffer;
	unsigned renderTexture;
	unsigned depthBuffer;
private:
	void drawAxis();
};

#endif