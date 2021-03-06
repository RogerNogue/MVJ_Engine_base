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
	void Draw();
	update_status PostUpdate()override;
	bool CleanUp();
	void WindowResized(unsigned width, unsigned height);
	ComponentMesh createComponentMesh(GameObject* dad);
	void UpdateEditorCamera();
	void setUpViewport();
	//variables
	void* context = nullptr;
	unsigned editorCameraBuffer = 0;
	unsigned editorTexture = 0;
	unsigned int frameBuffer = 0;
	unsigned int renderTexture = 0;
	unsigned int depthBuffer = 0;
	unsigned gizmoTransf = 0;
private:
	void RenderMeshes();
	void RenderShapes();
	void RenderWithNormals();
	void RenderWithNoNormals();
	void drawGizmo();

	//variables
	float screenWidth = 0;
	float screenHeight = 0;
	float texWidth = 0;
	float texHeight = 0;

};

#endif