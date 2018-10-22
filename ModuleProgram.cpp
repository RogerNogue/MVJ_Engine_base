#include "ModuleProgram.h"
#include "GL/glew.h"

ModuleProgram::ModuleProgram()
{
}


ModuleProgram::~ModuleProgram()
{
}

bool            ModuleProgram::Init() {
	char* dataVertex = nullptr;
	FILE* file = nullptr;
	fopen_s(&file, "Default.vs", "rb");
	if (file)
	{
		fseek(file, 0, SEEK_END);
		int size = ftell(file);
		rewind(file);
		char* dataVertex = (char*)malloc(size + 1);
		fread(dataVertex, 1, size, file);
		dataVertex[size] = 0;
		fclose(file);
	}

	char* dataFragment = nullptr;
	file = nullptr;
	fopen_s(&file, "Default.vs", "rb");
	if (file)
	{
		fseek(file, 0, SEEK_END);
		int size = ftell(file);
		rewind(file);
		char* dataFragment = (char*)malloc(size + 1);
		fread(dataFragment, 1, size, file);
		dataFragment[size] = 0;
		fclose(file);
	}
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	/*
	glShaderSource(vs, 1, &dataVertex, 0);
	glShaderSource(fs, 1, &dataFragment, 0);
	glCompileShader(vs);
	*/
	GLint params;

	glGetShaderiv(vs, GL_COMPILE_STATUS, &params);
	//glGetShaderInfoLog(vs, params, infolog);
	glCompileShader(fs);

	

	glDeleteShader(GL_VERTEX_SHADER);
	glDeleteShader(GL_FRAGMENT_SHADER);

	return true;
}
update_status   ModuleProgram::Update() {
	return UPDATE_CONTINUE;
}
bool            ModuleProgram::CleanUp() {
	return true;
}
