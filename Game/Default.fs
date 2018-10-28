#version 330 core
out vec4 color;

uniform vec4 newColor;
void main()
{
//if(vColor) color = vColor;
color = newColor;
}