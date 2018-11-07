#include "ModuleCamera.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleMenu.h"
#include "SDL.h"


ModuleCamera::ModuleCamera()
{
}


ModuleCamera::~ModuleCamera()
{
}

void ModuleCamera::camTransformation(math::float3 pos) {
	math::float4x4 transfMat = math::float4x4::identity;
	transfMat[0][3] = pos.x; transfMat[1][3] = pos.y; transfMat[2][3] = pos.z;
	camTransf = camTransf * transfMat;
}
void ModuleCamera::camRotationX(float angle) {
	math::float4x4 rotX = math::float4x4::identity;
	rotX[0][0] = 1; rotX[0][1] = 0;					rotX[0][2] = 0;
	rotX[1][0] = 0; rotX[1][1] = SDL_cos(angle);	rotX[1][2] = -SDL_sin(angle);
	rotX[2][0] = 0; rotX[2][1] = SDL_sin(angle);	rotX[2][2] = SDL_cos(angle);
	camTransf = rotX * camTransf;
}
void ModuleCamera::camRotationY(float angle) {
	math::float4x4 rotY = math::float4x4::identity;
	rotY[0][0] = SDL_cos(angle);	rotY[0][1] = 0;					rotY[0][2] = SDL_sin(angle);
	rotY[1][0] = 0;					rotY[1][1] = 1;					rotY[1][2] = 0;
	rotY[2][0] = -SDL_sin(angle);	rotY[2][1] = 0;					rotY[2][2] = SDL_cos(angle);
	camTransf = rotY * camTransf;
}
void ModuleCamera::camRotationZ(float angle) {
	math::float4x4 rotZ = math::float4x4::identity;
	rotZ[0][0] = math::Cos(angle);	rotZ[0][1] = -math::Sin(angle);	rotZ[0][2] = 0;
	rotZ[1][0] = math::Sin(angle);	rotZ[1][1] = math::Cos(angle);	rotZ[1][2] = 0;
	rotZ[2][0] = 0;					rotZ[2][1] = 0;					rotZ[2][2] = 1;
	camTransf = rotZ * camTransf;
}
void ModuleCamera::camSetUp() {
	camTransf = math::float4x4::identity;
	side = (fwd.Cross(up));
	up = (side.Cross(fwd));
	camTransf[0][0] = side.x;			camTransf[0][1] = side.y;			camTransf[0][2] = side.z;			camTransf[3][0] = 0;
	camTransf[1][0] = up.x;				camTransf[1][1] = up.y;				camTransf[1][2] = up.z;				camTransf[3][1] = 0;
	camTransf[2][0] = -fwd.x;			camTransf[2][1] = -fwd.y;			camTransf[2][2] = -fwd.z;			camTransf[3][2] = 0;
	camTransf[0][3] = -side.Dot(cam);	camTransf[1][3] = -up.Dot(cam);		camTransf[2][3] = fwd.Dot(cam);		camTransf[3][3] = 1;
	up.Normalize();
	side.Normalize();
}

bool            ModuleCamera::Init() {
	movementSpeed = 0.1;
	cam = math::float3(0, -2, 3);
	vrp = math::float3(0, 0, 0);
	up = math::float3(0, 1, 0);
	fwd = (vrp - cam);
	fwd.Normalize();
	camSetUp();
	timeLastFrame = SDL_GetTicks();
	return true;
}
update_status   ModuleCamera::Update() {
	//keyboard listeners
	if (App->input->keyboard[SDL_SCANCODE_LSHIFT]) {
		movementSpeed = 5;
	}
	else movementSpeed = 1;
	movementSpeed *= 1000/(SDL_GetTicks() - timeLastFrame);

	if (App->input->keyboard[SDL_SCANCODE_Q]) {
		camTransformation(up*movementSpeed);
	}
	if (App->input->keyboard[SDL_SCANCODE_E]) {
		camTransformation(-up * movementSpeed);
	}
	if (App->input->keyboard[SDL_SCANCODE_W]) {
		fwd.x = camTransf[2][0]; fwd.y = camTransf[2][1]; fwd.z = camTransf[2][2];
		camTransformation(fwd * movementSpeed);
	}
	if (App->input->keyboard[SDL_SCANCODE_A]) {
		side.x = camTransf[0][0]; side.y = camTransf[0][1]; side.z = camTransf[0][2];
		camTransformation(side * movementSpeed);
	}
	if (App->input->keyboard[SDL_SCANCODE_S]) {
		fwd.x = camTransf[2][0]; fwd.y = camTransf[2][1]; fwd.z = camTransf[2][2];
		camTransformation(-fwd * movementSpeed);
	}
	if (App->input->keyboard[SDL_SCANCODE_D]) {
		side.x = camTransf[0][0]; side.y = camTransf[0][1]; side.z = camTransf[0][2];
		camTransformation(-side * movementSpeed);
	}

	//arrows to rotate the camera
	if (App->input->keyboard[SDL_SCANCODE_UP]) {
		camRotationX(-movementSpeed * 0.25);
	}
	if (App->input->keyboard[SDL_SCANCODE_DOWN]) {
		camRotationX(movementSpeed * 0.25);
	}
	if (App->input->keyboard[SDL_SCANCODE_LEFT]) {
		camRotationY(-movementSpeed * 0.25);
	}
	if (App->input->keyboard[SDL_SCANCODE_RIGHT]) {
		camRotationY(movementSpeed * 0.25);
	}
	cam = { camTransf[3][0], camTransf[3][1], camTransf[3][2] };
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
	/*if (cameraChanged) {
		char* b = new char[50];
		sprintf(b, "Camera moved! New values:\n");
		App->menu->console.AddLog(b);
		sprintf(b, "Camera position: %.2f, %.2f, %.2f \n", cam.x, cam.y, cam.z);
		App->menu->console.AddLog(b);
		sprintf(b, "VRP position: %.2f, %.2f, %.2f \n", vrp.x, vrp.y, vrp.z);
		App->menu->console.AddLog(b);
		sprintf(b, "Up values: %.2f, %.2f, %.2f \n", up.x, up.y, up.z);
		App->menu->console.AddLog(b);
		sprintf(b, "Forward valuse: %.2f, %.2f, %.2f \n", fwd.x, fwd.y, fwd.z);
		App->menu->console.AddLog(b);
		sprintf(b, "Side valuse: %.2f, %.2f, %.2f \n\n", side.x, side.y, side.z);
		App->menu->console.AddLog(b);
		cameraChanged = false;
		delete b;
	}*/
	return UPDATE_CONTINUE;
}
bool            ModuleCamera::CleanUp() {
	return true;
}

void ModuleCamera::lookAt() {
	App->renderer->view = camTransf;
}

void ModuleCamera::lookAt(const math::float3& obs, const math::float3& vrp, math::float3& up) {
	fwd = math::float3(vrp - obs); fwd.Normalize();
	side = (fwd.Cross(up));
	math::float3 newUp = (side.Cross(fwd));
	math::float4x4 view;
	//now that we have all the values, we generate the view matrix
	view[0][0] = side.x; view[0][1] = side.y; view[0][2] = side.z; view[3][0] = 0;
	view[1][0] = newUp.x; view[1][1] = newUp.y; view[1][2] = newUp.z; view[3][1] = 0;
	view[2][0] = fwd.x; view[2][1] = fwd.y; view[2][2] = fwd.z; view[3][2] = 0;
	view[0][3] = -side.Dot(obs); view[1][3] = -newUp.Dot(obs); view[2][3] = fwd.Dot(obs); view[3][3] = 1;

	App->renderer->view = view;
	//updating cam values
	App->camera->up = newUp.Normalized();
	fwd = fwd.Normalized();
	side = side.Normalized();
}

void ModuleCamera::setFoV(float fov){

}

void ModuleCamera::SetAspectRatio(float aspect) {

}