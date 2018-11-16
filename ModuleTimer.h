#ifndef __ModuleTimer_H__
#define __ModuleTimer_H__

#include "Module.h"
#include "SDL.h"
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
	double getRealHighPrecisionTime();

	//variables
	float timeScale;
	unsigned frameCount;
	double initialTime;
	double realDeltaTime;
	double deltaTime;
	double lastFrameTime;

	//high precision variables
	double initialHighTime;
	static const double highFreq;
private:
	
};

#endif