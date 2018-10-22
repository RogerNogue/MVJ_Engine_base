#ifndef __ModuleMenu_h__
#define __ModuleMenu_h__
#include "Module.h"

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
};

#endif /* __ModuleMenu_h__ */
