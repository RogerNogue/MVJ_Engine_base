#ifndef __ModuleMenu_h__
#define __ModuleMenu_h__
#include "Module.h"
#include "Globals.h"

class ModuleMenu :
	public Module
{
public:
	ModuleMenu();
	~ModuleMenu();

	bool            Init();
	update_status   PreUpdate();
	update_status   Update();
	update_status   PostUpdate();
	bool            CleanUp();

	//variables
	int frames;
	double lastFrameTime;
	double lastSecondTime;
	int logMSIterator;
	int logFPSIterator;
	float* fps_log;
	float* ms_log;
private:
	void updateFramerates();
};

#endif /* __ModuleMenu_h__ */
