#pragma once
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleProgram.h"
#include "ModuleMenu.h"
#include "ModuleCamera.h"
#include "ModuleModelLoader.h"
#include "ModuleTimer.h"
#include "ModuleScene.h"
#include "Brofiler.h"
#include "ModuleDebugDraw.h"
#include "pcg_variants.h"
#include <time.h>

pcg32_random_t random;

using namespace std;

Application::Application()
{
	// Order matters: they will Init/start/update in this order
	modules.push_back(input = new ModuleInput());
	modules.push_back(window = new ModuleWindow());
	modules.push_back(scene = new ModuleScene());
	modules.push_back(modelLoader = new ModuleModelLoader());
	modules.push_back(menu = new ModuleMenu());
	modules.push_back(camera = new ModuleCamera());
	modules.push_back(textures = new ModuleTextures());
	modules.push_back(renderer = new ModuleRender());
	modules.push_back(shaderProgram = new ModuleProgram());
	modules.push_back(timer = new ModuleTimer());
	modules.push_back(debugDraw = new ModuleDebugDraw());
}

Application::~Application()
{
	for(list<Module*>::iterator it = modules.begin(); it != modules.end(); ++it)
    {
        delete *it;
    }
}

bool Application::Init()
{
	{ BROFILER_CATEGORY("Inits SHABALE", Profiler::Color::Peru)
		bool ret = true;
		pcg32_srandom_r(&random, time(NULL), (intptr_t)&random);

		for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret; ++it)
			ret = (*it)->Init();
		return ret;
	}
}

unsigned int Application::generateID() {
	return pcg32_random_r(&random);
}

update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		ret = (*it)->PreUpdate();

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		ret = (*it)->Update();

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		ret = (*it)->PostUpdate();
	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;

	for(list<Module*>::reverse_iterator it = modules.rbegin(); it != modules.rend() && ret; ++it)
		ret = (*it)->CleanUp();

	return ret;

}
