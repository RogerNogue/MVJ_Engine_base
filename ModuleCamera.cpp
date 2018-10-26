#include "ModuleCamera.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleRenderExercise.h"
#include "ModuleInput.h"
#include "ModuleMenu.h"
#include "SDL.h"


ModuleCamera::ModuleCamera()
{
}


ModuleCamera::~ModuleCamera()
{
}

bool            ModuleCamera::Init() {
	cameraChanged = false;
	movementSpeed = 0.1;
	return true;
}
update_status   ModuleCamera::Update() {
	//keyboard lecture
	//const Uint8 *keyboard = NULL;
	//keyboard listeners
	if (App->input->keyboard[SDL_SCANCODE_Q]) {
		App->exercise->cam += App->exercise->up*movementSpeed;
		App->exercise->vrp += App->exercise->up*movementSpeed;
		cameraChanged = true;
	}
	if (App->input->keyboard[SDL_SCANCODE_E]) {
		App->exercise->cam -= App->exercise->up*movementSpeed;
		App->exercise->vrp -= App->exercise->up*movementSpeed;
		cameraChanged = true;
	}
	if (App->input->keyboard[SDL_SCANCODE_W]) {
		App->exercise->cam += App->exercise->fwd*movementSpeed;
		App->exercise->vrp += App->exercise->fwd*movementSpeed;
		cameraChanged = true;
	}
	if (App->input->keyboard[SDL_SCANCODE_A]) {
		App->exercise->cam -= App->exercise->side*movementSpeed;
		App->exercise->vrp -= App->exercise->side*movementSpeed;
		cameraChanged = true;
	}
	if (App->input->keyboard[SDL_SCANCODE_S]) {
		App->exercise->cam -= App->exercise->fwd*movementSpeed;
		App->exercise->vrp -= App->exercise->fwd*movementSpeed;
		cameraChanged = true;
	}
	if (App->input->keyboard[SDL_SCANCODE_D]) {
		App->exercise->cam += App->exercise->side*movementSpeed;
		App->exercise->vrp += App->exercise->side*movementSpeed;
		cameraChanged = true;
	}
	if (App->input->keyboard[SDL_SCANCODE_LSHIFT]) {
		movementSpeed = 2;
	}
	else movementSpeed = 0.1;

	//mouse listeners
	/*static SDL_Event event;
	while (SDL_PollEvent(&event) != 0)
	{
		switch (event.type)
		{
		case SDL_MOUSEBUTTONDOWN:
			mouse_buttons[event.button.button - 1] = KEY_DOWN;
			break;

		case SDL_MOUSEBUTTONUP:
			mouse_buttons[event.button.button - 1] = KEY_UP;
			break;

		case SDL_MOUSEMOTION:
			mouse_motion.x = event.motion.xrel / SCREEN_SIZE;
			mouse_motion.y = event.motion.yrel / SCREEN_SIZE;
			mouse.x = event.motion.x / SCREEN_SIZE;
			mouse.y = event.motion.y / SCREEN_SIZE;
			break;
		}
	}*/
	//output console log with camera info
	if (cameraChanged) {
		char* b = new char[50];
		sprintf(b, "Camera moved! New values:\n");
		App->menu->console.AddLog(b);
		sprintf(b, "Camera position: %.2f, %.2f, %.2f \n", App->exercise->cam.x, App->exercise->cam.y, App->exercise->cam.z);
		App->menu->console.AddLog(b);
		sprintf(b, "VRP position: %.2f, %.2f, %.2f \n", App->exercise->vrp.x, App->exercise->vrp.y, App->exercise->vrp.z);
		App->menu->console.AddLog(b);
		sprintf(b, "Up values: %.2f, %.2f, %.2f \n\n", App->exercise->up.x, App->exercise->up.y, App->exercise->up.z);
		App->menu->console.AddLog(b);
		cameraChanged = false;
		delete b;
	}
	return UPDATE_CONTINUE;
}
bool            ModuleCamera::CleanUp() {
	return true;
}

void ModuleCamera::lookAt(const math::float3& obs, const math::float3& vrp, const math::float3& up) {
	math::float3 forward = (vrp - obs);
	forward.Normalize();
	math::float3 side = (forward.Cross(up));
	math::float3 newUp = (side.Cross(forward));
	math::float4x4 view;
	//now that we have all the values, we generate the view matrix
	view[0][0] = side.x; view[0][1] = side.y; view[0][2] = side.z; view[3][0] = 0;
	view[1][0] = newUp.x; view[1][1] = newUp.y; view[1][2] = newUp.z; view[3][1] = 0;
	view[2][0] = forward.x; view[2][1] = forward.y; view[2][2] = forward.z; view[3][2] = 0;
	view[0][3] = -side.Dot(obs); view[1][3] = -newUp.Dot(obs); view[2][3] = forward.Dot(obs); view[3][3] = 1;
	App->exercise->view = view;
	//updating cam values
	App->exercise->up = newUp;
	App->exercise->fwd = forward;
	App->exercise->side = side;
}

void ModuleCamera::setFoV(float fov){

}

void ModuleCamera::SetAspectRatio(float aspect) {

}