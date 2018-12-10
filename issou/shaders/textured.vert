#version 450

layout (location  = 0) in vec3 position;
layout (location = 1) in vec2 vertexUV;
//layout (location = 2) in vec3 normal;

out vec2 UV;

// send only MVP instead of three components
// give AItems a pointer to view and projection so that they can compute and send MVP themselves

uniform mat4 MVP; // model * view * projection


void main()
{
	gl_Position = MVP * vec4(position, 1.0);
	UV = vertexUV;
	UV = vec2(1, 1);
}