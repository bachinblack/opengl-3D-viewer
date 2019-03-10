#version 450

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 color;


uniform mat4 modelView;
uniform mat4 projection;


void main()
{
	vec4 pos = modelView * vec4(vertexPosition, 1.0);
	
	gl_Position = projection * pos;
}