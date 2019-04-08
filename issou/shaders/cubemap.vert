#version 450

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 normal;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform bool drawSkyBox;
uniform vec3 camPos;


out vec3 reflectDir;
out vec3 refractDirR;
out vec3 refractDirG;
out vec3 refractDirB;
out float reflectFactor;


const float eta = .95;
const float etaR = .65;
const float etaG = .67;
const float etaB = .69;


void main()
{
	if (drawSkyBox) {
		reflectDir = vertexPosition;
	} else {
		vec3 P = mat3(model) * vertexPosition;
		vec3 N = normalize(mat3(model) * normal);
		vec3 V = normalize(camPos - P);
		
		float R = pow((1.52-1), 2) / pow(1.52 + 1, 2);

		reflectFactor = R + (1-R) * pow((1 - dot(V, N)), 5);

		reflectDir = reflect(-V, N);
		refractDirR =  refract(-V, N, etaR);
		refractDirG =  refract(-V, N, etaG);
		refractDirB =  refract(-V, N, etaB);
	
	}

	gl_Position = projection * view * model * vec4(vertexPosition, 1.0);
}