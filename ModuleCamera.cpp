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

math::float3 ModuleCamera::transformation(math::float3 point, math::float3 transf) {
	math::float4x4 transfMat = math::float4x4::identity;
	transfMat[0][3] = transf.x; transfMat[1][3] = transf.y; transfMat[2][3] = transf.z;
	math::float4 point4; point4[0] = point.x; point4[1] = point.y; point4[2] = point.z; point4[3] = 1;

	point4 = transfMat * point4;
	math::float3 res; res[0] = point4.x; res[1] = point4.y; res[2] = point4.z;
	return res;
}

void ModuleCamera::rotationX(math::float3& p, float angle) {
	math::float3x3 rotX;
	rotX[0][0] = 1; rotX[0][1] = 0;					rotX[0][2] = 0;
	rotX[1][0] = 0; rotX[1][1] = SDL_cos(angle);	rotX[1][2] = -SDL_sin(angle);
	rotX[2][0] = 0; rotX[2][1] = SDL_sin(angle);	rotX[2][2] = SDL_cos(angle);

	p = p * rotX;
	//p = transformation(p, App->exercise->cam);
	
}

void ModuleCamera::rotationY(math::float3& p, float angle) {
	math::float3x3 rotY;
	rotY[0][0] = math::Cos(angle);	rotY[0][1] = 0;					rotY[0][2] = math::Sin(angle);
	rotY[1][0] = 0;					rotY[1][1] = 1;					rotY[1][2] = 0;
	rotY[2][0] = -math::Sin(angle);	rotY[2][1] = 0;					rotY[2][2] = math::Cos(angle);
	p = rotY * p;
}

void ModuleCamera::rotationZ(math::float3& p, float angle) {
	math::float3x3 rotZ;
	rotZ[0][0] = math::Cos(angle);	rotZ[0][1] = -math::Sin(angle);	rotZ[0][2] = 0;
	rotZ[1][0] = math::Sin(angle);	rotZ[1][1] = math::Cos(angle);	rotZ[1][2] = 0;
	rotZ[2][0] = 0;					rotZ[2][1] = 0;					rotZ[2][2] = 1;
	p = rotZ * p;
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

	//arrows to rotate the camera
	if (App->input->keyboard[SDL_SCANCODE_UP]) {
		rotationX(App->exercise->up, -0.005);
		//App->exercise->up.Normalize();
		App->exercise->vrp = transformation(App->exercise->vrp, -App->exercise->cam); 
		rotationX(App->exercise->vrp, -0.005);
		App->exercise->vrp = transformation(App->exercise->vrp, App->exercise->cam);
		cameraChanged = true;
	}
	if (App->input->keyboard[SDL_SCANCODE_DOWN]) {
		rotationX(App->exercise->up, 0.005);
		App->exercise->vrp = transformation(App->exercise->vrp, -App->exercise->cam); 
		rotationX(App->exercise->vrp, 0.005);
		App->exercise->vrp = transformation(App->exercise->vrp, App->exercise->cam);
		cameraChanged = true;
	}
	if (App->input->keyboard[SDL_SCANCODE_LEFT]) {
		rotationY(App->exercise->side, 0.005);
		App->exercise->vrp = transformation(App->exercise->vrp, -App->exercise->cam);
		rotationY(App->exercise->vrp, -0.005);
		App->exercise->vrp = transformation(App->exercise->vrp, App->exercise->cam);
		cameraChanged = true;
	}
	if (App->input->keyboard[SDL_SCANCODE_RIGHT]) {
		rotationY(App->exercise->side, -0.005);
		App->exercise->vrp = transformation(App->exercise->vrp, -App->exercise->cam);
		rotationY(App->exercise->vrp, -0.005);
		App->exercise->vrp = transformation(App->exercise->vrp, App->exercise->cam);
		cameraChanged = true;
	}

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
	App->exercise->up = newUp.Normalized();
	App->exercise->fwd = forward.Normalized();
	App->exercise->side = side.Normalized();
}

void ModuleCamera::setFoV(float fov){

}

void ModuleCamera::SetAspectRatio(float aspect) {

}