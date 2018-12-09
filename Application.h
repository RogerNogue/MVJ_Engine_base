#ifndef __Application_H__
#define __Application_H__

#include<list>
#include "Globals.h"
#include "Module.h"

class ModuleRender;
class ModuleWindow;
class ModuleTextures;
class ModuleInput;
class ModuleProgram;
class ModuleMenu;
class ModuleCamera;
class ModuleModelLoader;
class ModuleTimer;
class ModuleScene;
class ModuleDebugDraw;

class Application
{
public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();
	unsigned int generateID();

public:
	ModuleRender* renderer = nullptr;
	ModuleWindow* window = nullptr;
	ModuleTextures* textures = nullptr;
	ModuleInput* input = nullptr;
	ModuleProgram* shaderProgram = nullptr;
	ModuleMenu* menu = nullptr;
	ModuleCamera* camera = nullptr;
	ModuleModelLoader* modelLoader = nullptr;
	ModuleTimer* timer = nullptr;
	ModuleScene* scene = nullptr;
	ModuleDebugDraw* debugDraw = nullptr;
private:
	std::list<Module*> modules;

};

extern Application* App;

#endif
