#include "ModuleTimer.h"
#include "SDL.h"



ModuleTimer::ModuleTimer()
{
}


ModuleTimer::~ModuleTimer()
{
}

bool            ModuleTimer::Init() {
	initialTime = lastFrameTime = SDL_GetTicks();
	frameCount = 0;
	timeScale = 1;
	return true;
}
update_status   ModuleTimer::Update() {
	++frameCount;
	realDeltaTime = lastFrameTime - SDL_GetTicks();
	deltaTime += realDeltaTime * timeScale;
	lastFrameTime = SDL_GetTicks();
	return UPDATE_CONTINUE;
}
bool            ModuleTimer::CleanUp() {
	return true;
}
double ModuleTimer::getRealTime() {
	return initialTime - SDL_GetTicks();
}