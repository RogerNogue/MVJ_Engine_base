#version 330 core

out vec4 color;

in vec2 uv0;

uniform sampler2D texture0;

uniform vec4 newColor;
void main()
{
color = texture2D(texture0, uv0);
}