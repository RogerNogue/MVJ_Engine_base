#version 330 core

out vec4 color;

in vec2 uv0;
in vec3 normal;
in vec3 vertPos;

uniform sampler2D texture0;

//light calculation parameters
uniform mat4 view;
/*
uniform vec4 object_color;
uniform float ambient;
uniform float shininess;

uniform float k_ambient;
uniform float k_diffuse;
uniform float k_specular;
*/


uniform vec4 newColor;
void main()
{
//color = texture2D(texture0, uv0);
color = vec4(normal, 1);
}