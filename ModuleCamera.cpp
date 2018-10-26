#include "ModuleCamera.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleRenderExercise.h"
#include "ModuleInput.h"
#include "SDL.h"


ModuleCamera::ModuleCamera()
{
}


ModuleCamera::~ModuleCamera()
{
}

bool            ModuleCamera::Init() {
	return true;
}
update_status   ModuleCamera::Update() {
	//keyboard lecture
	//const Uint8 *keyboard = NULL;

	if (App->input->keyboard[SDL_SCANCODE_Q]) {
		App->exercise->cam += App->exercise->up*0.1;
		App->exercise->vrp += App->exercise->up*0.1;
	}
	if (App->input->keyboard[SDL_SCANCODE_E]) {
		App->exercise->cam -= App->exercise->up*0.1;
		App->exercise->vrp -= App->exercise->up*0.1;
	}
	if (App->input->keyboard[SDL_SCANCODE_W]) {
		App->exercise->cam += App->exercise->fwd*0.1;
		App->exercise->vrp += App->exercise->fwd*0.1;
	}
	if (App->input->keyboard[SDL_SCANCODE_A]) {
		App->exercise->cam -= App->exercise->side*0.1;
		App->exercise->vrp -= App->exercise->side*0.1;
	}
	if (App->input->keyboard[SDL_SCANCODE_S]) {
		App->exercise->cam -= App->exercise->fwd*0.1;
		App->exercise->vrp -= App->exercise->fwd*0.1;
	}
	if (App->input->keyboard[SDL_SCANCODE_D]) {
		App->exercise->cam += App->exercise->side*0.1;
		App->exercise->vrp += App->exercise->side*0.1;
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