#include "Application.h"

#include "ModuleRenderExercise.h"
#include "ModuleWindow.h"

#include "GL/glew.h"
#include "SDL.h"
#include "MathGeoLib.h"

ModuleRenderExercise::ModuleRenderExercise()
{
}

ModuleRenderExercise::~ModuleRenderExercise()
{
}

bool ModuleRenderExercise::Init()
{
	model = math::float4x4::identity;

	//view matrix
	cam = math::float3(0, 5, 3);
	vrp = math::float3(0, 0, 0);
	up = math::float3(0, 1, 0);

	math::float3 forward = (vrp - cam);
	forward.Normalize();
	math::float3 side = (forward.Cross(up));
	math::float3 newUp = (side.Cross(forward));
	//now that we have all the values, we generate the view matrix
	view[0][0] = side.x; view[0][1] = side.y; view[0][2] = side.z; view[3][0] = 0;
	view[1][0] = newUp.x; view[1][1] = newUp.y; view[1][2] = newUp.z; view[3][1] = 0;
	view[2][0] = forward.x; view[2][1] = forward.y; view[2][2] = forward.z; view[3][2] = 0;
	view[0][3] = -side.Dot(cam); view[1][3] = -newUp.Dot(cam); view[2][3] = forward.Dot(cam); view[3][3] = 1;

	//projection matrix

	Frustum frustum;
	frustum.type = FrustumType::PerspectiveFrustum;
	frustum.pos = float3::zero;
	frustum.front = -float3::unitZ;
	frustum.up = float3::unitY;
	frustum.nearPlaneDistance = 0.1f;
	frustum.farPlaneDistance = 100.0f;
	frustum.verticalFov = math::pi / 4.0f;
	float aspect = SCREEN_WIDTH/ SCREEN_HEIGHT;
	frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * 0.5f) *aspect);

	projection = frustum.ProjectionMatrix();;
	
	//points of the triangle declaration
	math::float4 point1 = math::float4(-1.0f, -1.0f, 0.0f, 1);
	math::float4 point2 = math::float4(1.0f, -1.0f, 0.0f, 1);
	math::float4 point3 = math::float4(0.0f, 1.0f, 0.0f, 1);

	point1 = projection * view * model * point1;
	point2 = projection * view * model * point2;
	point3 = projection * view * model * point3;

	float vertex_buffer_data[] = {
		point1.x / point1.w, point1.y / point1.w, point1.z / point1.w,
		point2.x / point2.w, point2.y / point2.w, point2.z / point2.w,
		point3.x / point3.w,  point3.y / point3.w, point2.z / point3.w,
	};
	/*
	float vertex_buffer_data[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        0.0f,  1.0f, 0.0f,
	};
	*/


    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data), vertex_buffer_data, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return vbo;
}

update_status ModuleRenderExercise::Update()
{
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

