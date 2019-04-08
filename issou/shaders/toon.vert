#version 450

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 color;


uniform mat4 modelView;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat3 normalMatrix;


out vec3 fNormal;
out vec3 fPosition;


void main()
{
	fNormal = normalize(normalMatrix * normal);

	vec4 pos = (modelView) * vec4(vertexPosition, 1.0);
	fPosition = pos.xyz;
	
	gl_Position = projection * (modelView * vec4(vertexPosition, 1.0));
}