#version 450

layout (location  = 0) in vec3 vertexPosition; // set the attr. number
layout (location = 1) in vec3 vertexColor;

uniform mat4 MVP;

out vec3 fColor; // for sending vertex color to fragment shader


void main()
{
	gl_Position = MVP * vec4(vertexPosition, 1.0);
	fColor = vertexColor;
}