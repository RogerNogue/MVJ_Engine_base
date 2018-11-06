#include "Application.h"

#include "ModuleRenderExercise.h"
#include "ModuleProgram.h"
#include "ModuleWindow.h"
#include "ModuleTextures.h"
#include "ModuleModelLoader.h"

#include "GL/glew.h"
#include "SDL.h"
#include "MathGeoLib.h"
#include "ModuleCamera.h"

ModuleRenderExercise::ModuleRenderExercise()
{
}

ModuleRenderExercise::~ModuleRenderExercise()
{
}

bool ModuleRenderExercise::Init()
{/*
	model = math::float4x4::identity;
	//projection matrix
	frustum.type = FrustumType::PerspectiveFrustum;
	frustum.pos = float3::zero;
	frustum.front = -float3::unitZ;
	frustum.up = float3::unitY;
	frustum.nearPlaneDistance = 0.1f;
	frustum.farPlaneDistance = 100.0f;
	frustum.verticalFov = math::pi / 4.0f;
	float aspect = SCREEN_WIDTH / SCREEN_HEIGHT;
	frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * 0.5f) *aspect);
	projection = frustum.ProjectionMatrix();

	*/
	float vertex_buffer_data[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,

		0, 0,
		1, 0,
		0, 1,
		0, 1,
		1, 0,
		1, 1
	};

	/*glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data), vertex_buffer_data, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);*/

	
	//texture = false;

	//texture0 = App->textures->Load(activeTexture, false);
    return true;
}

update_status ModuleRenderExercise::Update()
{
	//App->camera->lookAt(App->camera->cam, App->camera->vrp, App->camera->up);
	//App->camera->lookAt();



	//App->modelLoader->drawModel();
	
   // glEnableVertexAttribArray(0);
	
    /*glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(
            0,                  // attribute 0
            3,                  // number of componentes (3 floats)
            GL_FLOAT,           // data type
            GL_FALSE,           // should be normalized?
            0,                  // stride
            (void*)0            // array buffer offset
            );*/
	
	//texture program
	/*glUseProgram(App->shaderProgram->programTexture);

	glUniformMatrix4fv(glGetUniformLocation(App->shaderProgram->programTexture,
		"model"), 1, GL_TRUE, &model[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(App->shaderProgram->programTexture,
		"view"), 1, GL_TRUE, &view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(App->shaderProgram->programTexture,
		"proj"), 1, GL_TRUE, &projection[0][0]);

	glActiveTexture(GL_TEXTURE0);

	glUniform1i(glGetUniformLocation(App->shaderProgram->programTexture, "texture0"), 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0,
		(void*)(sizeof(float) * 3 * 6) // buffer offset
	);*/

	//glDrawArrays(GL_TRIANGLES, 0, 6); // Starting from vertex 0; 3 vertices total -> 1 triangle

	//glDisableVertexAttribArray(0);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);

	/*glDisableVertexAttribArray(1);
	glBindTexture(GL_TEXTURE_2D, texture0);*/

	//glUseProgram(0);

	return UPDATE_CONTINUE;
}

bool ModuleRenderExercise::CleanUp()
{
    if(vbo != 0)
    {
        glDeleteBuffers(1, &vbo);
    }

	return true;
}

void ModuleRenderExercise::activateTextures(char* name) {
	//texture = true;
	//activeTexture = name;
}

void ModuleRenderExercise::aspectRatioChanged(const unsigned width, const unsigned height) {
	float aspect = width / height;
	frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * 0.5f) *aspect);
	projection = frustum.ProjectionMatrix();
}