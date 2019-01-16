#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleCamera.h"
#include "ModuleRender.h"
#include "ModuleMenu.h"
#include "SDL/include/SDL.h"
#include "Brofiler.h"
#include "ModuleScene.h"
#include "ModuleModelLoader.h"

ModuleInput::ModuleInput()
{}

// Destructor
ModuleInput::~ModuleInput()
{}

// Called before render is available
bool ModuleInput::Init()
{
	LOG("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	cameraMoved = false;
	rightclickPressed = false;
	firstFrameRightClick = true;
	wheelScroll = 0;
	return ret;
}
update_status ModuleInput::PreUpdate() {
	BROFILER_CATEGORY("Module input pre update", Profiler::Color::Orange)
	SDL_PumpEvents();

	keyboard = SDL_GetKeyboardState(NULL);

	if (keyboard[SDL_SCANCODE_ESCAPE]) {
		return UPDATE_STOP;
	}
	if (keyboard[SDL_SCANCODE_T])App->renderer->gizmoTransf = 0;
	if (keyboard[SDL_SCANCODE_R])App->renderer->gizmoTransf = 1;
	if (keyboard[SDL_SCANCODE_E])App->renderer->gizmoTransf = 2;
	SDL_Event sdlEvent;
	if (SDL_PollEvent(&sdlEvent) != 0) {
		// Esc button is pressed
		switch (sdlEvent.type)
		{
		case SDL_QUIT:
			return UPDATE_STOP;
			break;

		case SDL_WINDOWEVENT:
			if (sdlEvent.window.event == SDL_WINDOWEVENT_RESIZED || sdlEvent.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
				App->renderer->WindowResized(sdlEvent.window.data1, sdlEvent.window.data2);
			}
			break;
		case SDL_MOUSEBUTTONDOWN:
			//if right click, then the camera moves
			if (sdlEvent.button.button == SDL_BUTTON_RIGHT) {
				rightclickPressed = true;
			}
			//if left click, check 
			else if (sdlEvent.button.button == SDL_BUTTON_LEFT) {
				int mouseX, mouseY;
				SDL_GetMouseState(&mouseX, &mouseY);
				App->scene->mouseClick(mouseX, mouseY);
			}
			break;

		case SDL_MOUSEBUTTONUP:
			if (sdlEvent.button.button == SDL_BUTTON_RIGHT) {
				rightclickPressed = false;
				firstFrameRightClick = true;
			}
			break;

		case SDL_MOUSEMOTION:
			if (rightclickPressed) {
				if (firstFrameRightClick) {
					x = sdlEvent.motion.x;
					y = sdlEvent.motion.y;
					firstFrameRightClick = false;
					break;
				}
				cameraMoved = true;
				xdiff = sdlEvent.motion.x - x;
				ydiff = sdlEvent.motion.y - y;
				x = sdlEvent.motion.x;
				y = sdlEvent.motion.y;
			}
			break;

		case SDL_MOUSEWHEEL:
			wheelScroll = sdlEvent.wheel.y;
				break;

		case SDL_DROPFILE:
			App->modelLoader->loadDroppedFile(sdlEvent.drop.file);
			int i = 1;
			//App->modelLoader->loadModelFromPath(sdlEvent.drop.file);
			break;
		}
	}
	App;
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleInput::CleanUp()
{
	LOG("Quitting SDL input event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}