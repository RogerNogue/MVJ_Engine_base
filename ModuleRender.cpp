#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "SDL.h"
#include "GL/glew.h"
#include "MathGeoLib.h"
#include "ModuleModelLoader.h"
#include "ModuleProgram.h"
#include "ModuleCamera.h"
#include "ModuleTextures.h"
#include "ModuleMenu.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "GameObject.h"
#include "ModuleScene.h"
#include "debugdraw.h"
#include "Brofiler.h"

ModuleRender::ModuleRender()
{
}

// Destructor
ModuleRender::~ModuleRender()
{
}

// Called before render is available
bool ModuleRender::Init()
{
	LOG("Creating Renderer context");

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	context = SDL_GL_CreateContext(App->window->window);

	glewInit();

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glEnable(GL_TEXTURE_2D);

	glClearDepth(1.0f);
	glClearColor(0.3f, 0.3f, 0.3f, 1.f);

    int width, height;
    SDL_GetWindowSize(App->window->window, &width, &height);
    glViewport(0, 0, width, height);

	App->modelLoader->loadModel(1);
	UpdateEditorCamera();
	
	return true;
}

update_status ModuleRender::PreUpdate()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	return UPDATE_CONTINUE;
}
void ModuleRender::UpdateEditorCamera() {

	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

	glViewport(0, 0, App->camera->screenWidth, App->camera->screenHeight);
	glClearColor(0.2f, 0.2f, 0.2f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	/*glDeleteFramebuffers(1, &frameBuffer);
	glDeleteTextures(1, &renderTexture);
	glDeleteRenderbuffers(1, &depthBuffer);

	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

	glGenTextures(1, &renderTexture);

	glBindTexture(GL_TEXTURE_2D, renderTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, App->camera->screenWidth, App->camera->screenHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	depthBuffer;
	glGenRenderbuffers(1, &depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, App->camera->screenWidth, App->camera->screenHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderTexture, 0);


	//framebuffer creation
	glGenFramebuffers(1, &editorCameraBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, editorCameraBuffer);
	//texture creation
	editorTexture = App->textures->createFrameBuffer("EditorCam", App->camera->screenWidth, App->camera->screenHeight);
	*/

}

// Called every draw update
void ModuleRender::Draw()
{
	BROFILER_CATEGORY("UpdateRenderer", Profiler::Color::Chartreuse)

	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

	glViewport(0, 0, App->camera->screenWidth, App->camera->screenHeight);
	glClearColor(0.2f, 0.2f, 0.2f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//drawing the model
	for (unsigned j = 0; j < App->modelLoader->allMeshes.size(); ++j) {
		if (!App->modelLoader->allMeshes[j]->active) break;
		glUseProgram(App->shaderProgram->programTexture);
		glUniformMatrix4fv(glGetUniformLocation(App->shaderProgram->programTexture,
			"model"), 1, GL_TRUE, &App->camera->model[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(App->shaderProgram->programTexture,
			"view"), 1, GL_TRUE, &App->camera->view[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(App->shaderProgram->programTexture,
			"proj"), 1, GL_TRUE, &App->camera->projection[0][0]);
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, App->modelLoader->allMeshes[j]->dad->materials[App->modelLoader->allMeshes[j]->mesh.material]->material.texture0);
		glUniform1i(glGetUniformLocation(App->shaderProgram->programTexture, "texture0"), 0);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, App->modelLoader->allMeshes[j]->mesh.vbo);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float) * App->modelLoader->allMeshes[j]->mesh.numVertices * 3));

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, App->modelLoader->allMeshes[j]->mesh.vio);

		glDrawElements(GL_TRIANGLES, App->modelLoader->allMeshes[j]->mesh.numIndices, GL_UNSIGNED_INT, nullptr);


		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glUseProgram(0);
	}
	/*
	//drawing axis and grid
	glUseProgram(App->shaderProgram->programGeometry);

	glUniformMatrix4fv(glGetUniformLocation(App->shaderProgram->programGeometry,
		"model"), 1, GL_TRUE, &App->camera->model[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(App->shaderProgram->programGeometry,
		"view"), 1, GL_TRUE, &App->camera->view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(App->shaderProgram->programGeometry,
		"proj"), 1, GL_TRUE, &App->camera->projection[0][0]);

	int zAxis = glGetUniformLocation(App->shaderProgram->programGeometry, "newColor");
	float white[4] = { 1, 1, 1, 1 };
	glUniform4fv(zAxis, 1, white);

	//lines

	
		//dd::xzSquareGrid(-40.0f, 40.0f, 0.0f, 1.0f, math::float3(0.65f, 0.65f, 0.65f));
		//float axis_size = max(App->models->bsphere.radius, 1.0f);
		//dd::axisTriad(math::float4x4::identity, axis_size*0.125f, axis_size*1.25f, 0, false);
		//dd::axisTriad(math::float4x4::identity, 5*0.125f, 5*1.25f, 0, false);

	glLineWidth(1.0f);

	glBegin(GL_LINES);
	float d = 200.f;

	for (float i = -d; i <= d; i += 1.0f) {
		glVertex3f(i, 0.0f, -d);
		glVertex3f(i, 0.0f, d);
		glVertex3f(-d, 0.0f, i);
		glVertex3f(d, 0.0f, i);
	}
	glEnd();

	drawAxis();*/

	glUseProgram(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

update_status ModuleRender::PostUpdate()
{
	SDL_GL_SwapWindow(App->window->window);

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRender::CleanUp()
{
	LOG("Destroying renderer");

	//Destroy window

	return true;
}

void ModuleRender::WindowResized(unsigned width, unsigned height)
{
    glViewport(0, 0, width, height);
	App->camera->SetAspectRatio(width, height);
	UpdateEditorCamera();
}

void ModuleRender::drawAxis() {
	//x, red
	int xAxis = glGetUniformLocation(App->shaderProgram->programGeometry, "newColor");
	float red[4] = { 1, 0, 0, 1 };
	glUniform4fv(xAxis, 1, red);
	glLineWidth(2.5);
	glBegin(GL_LINES);
	glVertex3f(0, 0, 0); glVertex3f(5, 0, 0);
	glEnd();

	//y green
	int yAxis = glGetUniformLocation(App->shaderProgram->programGeometry, "newColor");
	float green[4] = { 0, 1, 0, 1 };
	glUniform4fv(yAxis, 1, green);

	glBegin(GL_LINES);
	glVertex3f(0, 0, 0); glVertex3f(0, 5, 0);
	glEnd();

	//z blue
	int zAxis = glGetUniformLocation(App->shaderProgram->programGeometry, "newColor");
	float blue[4] = { 0, 0, 1, 1 };
	glUniform4fv(zAxis, 1, blue);

	glBegin(GL_LINES);
	glVertex3f(0, 0, 0); glVertex3f(0, 0, 5);
	glEnd();
}

ComponentMesh ModuleRender::createComponentMesh(GameObject* dad) {
	ComponentMesh c(dad);
	return c;
}