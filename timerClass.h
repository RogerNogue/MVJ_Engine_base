#pragma once
#include"SDL.h"
class timerClass
{
public:
	timerClass() {
		lowRunning = false;
		highRunning = false;
	}
	~timerClass() {

	}
	void StartLowPrecision() {
		currentLowPrecisionTimer = SDL_GetTicks();
		lowRunning = true;
	}
	double ReadLowPrecision() {
		if(lowRunning)	return SDL_GetTicks() - currentLowPrecisionTimer;
		else return currentLowPrecisionTimer;
	}
	double StopLowPrecision() {
		lowRunning = false;
		currentLowPrecisionTimer = SDL_GetTicks() - currentLowPrecisionTimer;
		return currentLowPrecisionTimer;
	}
	void StartHighPrecision() {
		currentHighPrecisionTimer = SDL_GetPerformanceCounter();
		highRunning = true;
	}
	double ReadHighPrecision() {
		if(highRunning)	return (SDL_GetPerformanceCounter() - currentHighPrecisionTimer)*1000 / freq();
		else return currentHighPrecisionTimer;
	}
	double StopHighPrecision() {
		highRunning = false;
		currentHighPrecisionTimer = (SDL_GetPerformanceCounter() - currentHighPrecisionTimer)*1000 / freq();
		return currentHighPrecisionTimer;
	}
	//variable
private:
	double currentLowPrecisionTimer;
	double currentHighPrecisionTimer;
	bool lowRunning;
	bool highRunning;
};

