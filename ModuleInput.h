#ifndef __ModuleInput_H__
#define __ModuleInput_H__

#include "Module.h"
#include "Globals.h"

typedef unsigned __int8 Uint8;

class ModuleInput : public Module
{
public:
	
	ModuleInput();
	~ModuleInput();

	bool Init();
	update_status PreUpdate();
	bool CleanUp();
	//vars
	const Uint8 *keyboard = NULL;
	int x, y, xdiff, ydiff;
	bool cameraMoved;
private:
};

#endif