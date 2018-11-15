#include "ModuleCamera.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleModelLoader.h"
#include "ModuleInput.h"
#include "ModuleMenu.h"
#include "SDL.h"


ModuleCamera::ModuleCamera()
{
}


ModuleCamera::~ModuleCamera()
{
}

void ModuleCamera::camRotationX(float angle) {
	math::Quat rot = Quat::RotateAxisAngle(math::Cross(frustum.up, frustum.front), angle);
	frustum.front = (rot * frustum.front).Normalized();
	frustum.up = (rot * frustum.up).Normalized();
	updateCam();
}
void ModuleCamera::camRotationY(float angle) {
	math::Quat rot = Quat::RotateAxisAngle(math::float3(0, 1, 0), angle);
	frustum.front = (rot * frustum.front).Normalized();
	frustum.up = (rot * frustum.up).Normalized();
	updateCam();
}

void ModuleCamera::updateCam() {
	projection = frustum.ProjectionMatrix();
	view = frustum.ViewMatrix();
}

bool            ModuleCamera::Init() {
	movementSpeed = 1;
	vrp = math::float3(0, 0, 0);
	timeLastFrame = SDL_GetTicks();
	mouseRotSpeed = 0.001;
	movementOn = false;
	//drawing matrices
	model = math::float4x4::identity;
	//projection matrix
	frustum.type = FrustumType::PerspectiveFrustum;
	frustum.pos = math::float3(0, 4, -7);
	frustum.front = (vrp - frustum.pos).Normalized();
	frustum.up = math::Cross(frustum.front, math::float3(1, 0, 0));
	frustum.nearPlaneDistance = 0.1f;
	frustum.farPlaneDistance = 100.0f;
	frustum.verticalFov = math::pi / 4.0f;
	screenWidth = SCREEN_WIDTH;
	screenHeight = SCREEN_HEIGHT;
	aspectRatio = screenWidth / screenHeight;
	frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * 0.5f) *aspectRatio);
	projection = frustum.ProjectionMatrix();
	view = frustum.ViewMatrix();

	return true;
}
update_status   ModuleCamera::Update() {
	if (movementOn) {
		//keyboard listeners
		if (App->input->keyboard[SDL_SCANCODE_LSHIFT]) {
			movementSpeed = 5.;
		}
		else movementSpeed = 1.;
		movementSpeed *= 1000 / (SDL_GetTicks() - timeLastFrame);

		if (App->input->keyboard[SDL_SCANCODE_Q]) {
			frustum.pos -= frustum.up * movementSpeed;
			vrp -= math::float3(0, 1, 0) * movementSpeed;
			frustum.front = (vrp - frustum.pos).Normalized();
			updateCam();
		}
		if (App->input->keyboard[SDL_SCANCODE_E]) {
			frustum.pos += frustum.up * movementSpeed;
			vrp += math::float3(0, 1, 0) * movementSpeed;
			frustum.front = (vrp - frustum.pos).Normalized();
			updateCam();
		}
		if (App->input->keyboard[SDL_SCANCODE_W]) {
			frustum.pos += frustum.front * movementSpeed;
			updateCam();
		}
		if (App->input->keyboard[SDL_SCANCODE_A]) {
			float3 side = frustum.front.Cross(frustum.up);
			frustum.pos -= side * movementSpeed;
			updateCam();
		}
		if (App->input->keyboard[SDL_SCANCODE_S]) {
			frustum.pos -= frustum.front * movementSpeed;
			updateCam();
		}
		if (App->input->keyboard[SDL_SCANCODE_D]) {
			float3 side = frustum.front.Cross(frustum.up);
			frustum.pos += side * movementSpeed;
			updateCam();
		}
		/*
		//arrows to rotate the camera
		if (App->input->keyboard[SDL_SCANCODE_UP]) {
			camRotationX(movementSpeed / 5);
		}
		if (App->input->keyboard[SDL_SCANCODE_DOWN]) {
			camRotationX(-movementSpeed / 5);
		}
		if (App->input->keyboard[SDL_SCANCODE_LEFT]) {
			camRotationY(movementSpeed / 5);
		}
		if (App->input->keyboard[SDL_SCANCODE_RIGHT]) {
			camRotationY(-movementSpeed / 5);
		}*/
		//camera rotation via mouse
		if (App->input->cameraMoved) {
			math::float3 distCamModel = frustum.pos - modelCenter;
			//case orbit instead of rotation
			if (App->input->keyboard[SDL_SCANCODE_LALT]) {
				math::Quat rotx = Quat::RotateAxisAngle(math::Cross(frustum.up, frustum.front), App->input->ydiff * mouseRotSpeed);
				math::Quat roty = Quat::RotateAxisAngle(math::float3(0, 1, 0), App->input->xdiff * mouseRotSpeed);

				frustum.pos = modelCenter + roty * rotx * distCamModel;
				frustum.front = (modelCenter - frustum.pos).Normalized();
			}
			else {
				camRotationX(App->input->ydiff * mouseRotSpeed);
				camRotationY(App->input->xdiff * mouseRotSpeed * -1);
			}
			App->input->cameraMoved = false;
			updateCam();
		}
	}
	//mousewheel
	if (App->input->wheelScroll != 0) {
		frustum.verticalFov -= 0.1 * App->input->wheelScroll;
		frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * 0.5f) *aspectRatio);
		App->input->wheelScroll = 0;
		updateCam();
	}
	return UPDATE_CONTINUE;
}
bool            ModuleCamera::CleanUp() {
	return true;
}

void ModuleCamera::SetAspectRatio(float w, float h) {
	screenWidth = w;
	screenHeight = h;
	aspectRatio = w/h;
	frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * 0.5f) *aspectRatio);
	projection = frustum.ProjectionMatrix();
	view = frustum.ViewMatrix();
}

void ModuleCamera::mewModelLoaded() {
	//not correct
	modelCenter.x = App->modelLoader->minX + (App->modelLoader->maxX - App->modelLoader->minX)/2;
	modelCenter.y = App->modelLoader->minY + (App->modelLoader->maxY - App->modelLoader->minY) / 2;
	modelCenter.z = App->modelLoader->minZ;
	vrp = modelCenter;
	modelWidth = (App->modelLoader->maxY - App->modelLoader->minY)/2 / math::Tan(frustum.verticalFov/2);
	modelHeight = (App->modelLoader->maxX - App->modelLoader->minX) / 2 / math::Tan(frustum.horizontalFov / 2);

	frustum.pos = modelCenter - (frustum.front * max(modelWidth, modelHeight));
	frustum.front = (modelCenter - frustum.pos).Normalized();
	updateCam();
}