#version 330

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec2 vertex_uv0;
out vec2 uv0;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;
void main()
{
 uv0 = vertex_uv0;
 gl_Position = proj*view*model*vec4(vertex_position, 1.0);
}
