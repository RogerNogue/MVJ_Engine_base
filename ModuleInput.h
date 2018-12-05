#ifndef __ModuleInput_H__
#define __ModuleInput_H__

#include "Module.h"

typedef unsigned __int8 Uint8;

class ModuleInput : public Module
{
public:
	
	ModuleInput();
	~ModuleInput();

	bool Init() override;
	update_status PreUpdate() override;
	bool CleanUp() override;
	//vars
	int wheelScroll;
	const Uint8 *keyboard = nullptr;
	int x, y, xdiff, ydiff;
	bool cameraMoved;
	bool rightclickPressed;
	bool firstFrameRightClick;
private:
};

#endif