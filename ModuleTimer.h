#pragma once
#include "Module.h"
class ModuleTimer :
	public Module
{
public:
	ModuleTimer();
	~ModuleTimer();

	bool            Init();
	update_status   Update();
	bool            CleanUp();
	double getRealTime();

	//variables
	float timeScale;
	unsigned frameCount;
	double initialTime;
	double realDeltaTime;
	double deltaTime;
	double lastFrameTime;
private:
	
};

