#version 450

layout (location  = 0) in vec3 vertexPosition; // set the attr. number
layout (location = 1) in vec3 vertexColor;

out vec3 fColor; // for sending vertex color to fragment shader
uniform mat4 MVP;


void main()
{
	gl_Position = MVP * vec4(vertexPosition, 1.0);
	fColor = vertexColor;
}