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

	App->modelLoader->loadModel(2);
	App->modelLoader->drawModel();

	return true;
}

update_status ModuleRender::PreUpdate()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleRender::Update()
{
	//App->camera->lookAt();
	//drawing the model
	unsigned numMeshes = App->modelLoader->meshes.size();
	for (unsigned i = 0; i < numMeshes; ++i) {
		//geometry shaders

		glUseProgram(App->shaderProgram->programTexture);
		glUniformMatrix4fv(glGetUniformLocation(App->shaderProgram->programTexture,
			"model"), 1, GL_TRUE, &App->camera->model[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(App->shaderProgram->programTexture,
			"view"), 1, GL_TRUE, &App->camera->view[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(App->shaderProgram->programTexture,
			"proj"), 1, GL_TRUE, &App->camera->projection[0][0]);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, App->modelLoader->materials[App->modelLoader->meshes[i].material].texture0);
		glUniform1i(glGetUniformLocation(App->shaderProgram->programTexture, "texture0"), 0);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, App->modelLoader->meshes[i].vbo);
		glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,(void*)0 );
		glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,0,(void*)(sizeof(float) * App->modelLoader->meshes[i].numVertices * 3));

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, App->modelLoader->meshes[i].vio);

		if (App->modelLoader->modelGeometry == 0)glDrawElements(GL_TRIANGLES, App->modelLoader->meshes[i].numIndices, GL_UNSIGNED_INT, nullptr);
		else if (App->modelLoader->modelGeometry == 1)glDrawElements(GL_POLYGON, App->modelLoader->meshes[i].numIndices, GL_UNSIGNED_INT, nullptr);
		else if (App->modelLoader->modelGeometry == 2)glDrawElements(GL_POLYGON, App->modelLoader->meshes[i].numIndices, GL_UNSIGNED_INT, nullptr);
		
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glUseProgram(0);
	}
	
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

	drawAxis();
	
	glUseProgram(0);

	return UPDATE_CONTINUE;
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
    glViewport(0, 0, width, height); //diagonal resize not working
	App->camera->SetAspectRatio(width, height);
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
