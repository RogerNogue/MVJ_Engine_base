#ifndef __Globals_H__
#define __Globals_H__

#include <windows.h>
#include <stdio.h>

#define LOG(format, ...) log(__FILE__, __LINE__, format, __VA_ARGS__);

void log(const char file[], int line, const char* format, ...);

enum update_status
{
	UPDATE_CONTINUE = 1,
	UPDATE_STOP,
	UPDATE_ERROR
};

enum component_type {
	CAMERA,
	MESH,
	MATERIAL,
	TRANSFORM,
	OBJECT
};

// Configuration -----------
#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 1000
#define FULLSCREEN false
#define BORDERLESS false
#define RESIZABLE_WINDOW true
#define VSYNC true
#define TITLE "GNOBLIN"
#define GLSL_VERSION "#version 330"

#endif