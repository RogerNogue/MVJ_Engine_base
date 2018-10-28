#include "Application.h"

#include "ModuleRenderExercise.h"
#include "ModuleProgram.h"
#include "ModuleWindow.h"

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
{
	model = math::float4x4::identity;
	
    return true;
}

update_status ModuleRenderExercise::Update()
{
	App->camera->lookAt(App->camera->cam, App->camera->vrp, App->camera->up);

	//projection matrix
	Frustum frustum;
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

	
	glUniformMatrix4fv(glGetUniformLocation(App->shaderProgram->program,
		"model"), 1, GL_TRUE, &model[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(App->shaderProgram->program,
		"view"), 1, GL_TRUE, &view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(App->shaderProgram->program,
		"proj"), 1, GL_TRUE, &projection[0][0]);
	
	int zAxis = glGetUniformLocation(App->shaderProgram->program, "newColor");
	float white[4] = { 1, 1, 1, 1 };
	glUniform4fv(zAxis, 1, white);

	float vertex_buffer_data[] = {
	-1.0f, -1.0f, 0.0f,
	1.0f, -1.0f, 0.0f,
	0.0f,  1.0f, 0.0f,
	};

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data), vertex_buffer_data, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

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

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(
            0,                  // attribute 0
            3,                  // number of componentes (3 floats)
            GL_FLOAT,           // data type
            GL_FALSE,           // should be normalized?
            0,                  // stride
            (void*)0            // array buffer offset
            );

    glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle

    glDisableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

	drawAxis();

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

void ModuleRenderExercise::drawAxis() {
	//x, red
	int xAxis = glGetUniformLocation(App->shaderProgram->program, "newColor");
	float red[4] = { 1, 0, 0, 1 };
	glUniform4fv(xAxis, 1, red);
	glLineWidth(2.5);
	glBegin(GL_LINES);
	glVertex3f(0, 0, 0); glVertex3f(5, 0, 0);
	glEnd();

	//y green
	int yAxis = glGetUniformLocation(App->shaderProgram->program, "newColor");
	float green[4] = { 0, 1, 0, 1 };
	glUniform4fv(yAxis, 1, green);

	glBegin(GL_LINES);
	glVertex3f(0, 0, 0); glVertex3f(0, 5, 0);
	glEnd();

	//z blue
	int zAxis = glGetUniformLocation(App->shaderProgram->program, "newColor");
	float blue[4] = { 0, 0, 1, 1 };
	glUniform4fv(zAxis, 1, blue);

	glBegin(GL_LINES);
	glVertex3f(0, 0, 0); glVertex3f(0, 0, 5);
	glEnd();
}
