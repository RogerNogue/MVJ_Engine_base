#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleCamera.h"
#include "ModuleRender.h"
#include "ModuleMenu.h"
#include "SDL/include/SDL.h"

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
	wheelScroll = 0;
	return ret;
}
update_status ModuleInput::PreUpdate() {
	SDL_PumpEvents();

	keyboard = SDL_GetKeyboardState(NULL);

	if (keyboard[SDL_SCANCODE_ESCAPE]) {
		return UPDATE_STOP;
	}
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
				LOG("right click pressed\n");
				App->camera->movementOn = true;
			}
			//if left click, check 
			/*else if () {

			}*/
			break;

		case SDL_MOUSEBUTTONUP:
			if (sdlEvent.button.button == SDL_BUTTON_RIGHT) {
				App->camera->movementOn = false;
				LOG("right click released\n");
			}
			break;

		case SDL_MOUSEMOTION:
			if (!App->camera->movementOn) {
				x = sdlEvent.motion.x;
				y = sdlEvent.motion.y;
				break;
			}
			cameraMoved = true;
			xdiff = sdlEvent.motion.x - x;
			ydiff = sdlEvent.motion.y - y;
			x = sdlEvent.motion.x;
			y = sdlEvent.motion.y;
			break;

		case SDL_MOUSEWHEEL:
			wheelScroll = sdlEvent.wheel.y;
		}
	}
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleInput::CleanUp()
{
	LOG("Quitting SDL input event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}