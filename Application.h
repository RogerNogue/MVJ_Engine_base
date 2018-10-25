#pragma once

#include<list>
#include "Globals.h"
#include "Module.h"
#include <time.h>

class ModuleRender;
class ModuleWindow;
class ModuleTextures;
class ModuleInput;
class ModuleRenderExercise;
class ModuleProgram;
class ModuleMenu;

class Application
{
public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();
	double currentTimeMS();
	int currentTimeS();

public:
	ModuleRender* renderer = nullptr;
	ModuleWindow* window = nullptr;
	ModuleTextures* textures = nullptr;
	ModuleInput* input = nullptr;
    ModuleRenderExercise* exercise = nullptr;
	ModuleProgram* shaderProgram = nullptr;
	ModuleMenu* menu = nullptr;
private:
	clock_t startingTime;
	std::list<Module*> modules;

};

extern Application* App;
