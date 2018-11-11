#ifndef __ModuleProgram_H__
#define __ModuleProgram_H__

#include "Module.h"
#include "GL/glew.h"
class ModuleProgram :
	public Module
{
public:
	ModuleProgram();
	~ModuleProgram();

	bool            Init();
	update_status   Update();
	bool            CleanUp();

	GLuint loadShaders(char* dataVertex, char* dataFragment);

	GLuint programTexture;
	GLuint programGeometry;
};

#endif