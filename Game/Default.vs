#version 330

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec2 vertex_uv0;
layout(location = 2) in vec3 vertex_normal;
out vec2 uv0;
out vec3 normal;
out vec3 vertPos;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;
void main()
{
 uv0 = vertex_uv0;
 vertPos	 = (model*vec4(vertex_position, 1.0)).xyz;
 normal      = (model*vec4(vertex_normal, 0.0)).xyz;

 gl_Position = proj*view*model*vec4(vertex_position, 1.0);
}
