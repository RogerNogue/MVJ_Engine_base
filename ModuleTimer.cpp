#include "ModuleTimer.h"
#include "SDL.h"

const double ModuleTimer::highFreq = SDL_GetPerformanceFrequency();

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
	initialHighTime = SDL_GetPerformanceCounter();
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

double ModuleTimer::getRealHighPrecisionTime() {
	return ((SDL_GetPerformanceCounter() - initialHighTime) * 1000 / highFreq);
}