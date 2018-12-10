#version 450

// layout = VBO (first parameter in glVertexAttribPointer)
// attribute = in = from code
// varying = out = to fragment shader

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 normal;
//layout (location = 2) in vec3 color;

out vec3 fColor;
uniform mat4 modelView;
uniform mat4 projection;

uniform vec4 lightLoc;
uniform vec3 Kd;
uniform vec3 Ld;

uniform mat3 normalMatrix;

out vec3 fNormal;
out vec3 fPosition;

out vec3 LightIntensity; 

void main()
{
	fNormal = normalize(normalMatrix * normal);
	vec3 N = fNormal;
	
	vec4 P = modelView * vec4(vertexPosition, 1.0);
	vec3 L = normalize(vec3(lightLoc - P));
	
	vec3 diffuse = Kd * Ld * max(dot(L, N), 0.0);

	fPosition = P.xyz;
	
	LightIntensity = diffuse;

	gl_Position = projection * P;
}

// shdr
//precision highp float;
//attribute vec3 position;
//attribute vec3 normal;
//uniform mat3 normalMatrix;
//uniform mat4 modelViewMatrix;
//uniform mat4 projectionMatrix;
//varying vec3 fNormal;
//varying vec3 fPosition;
//varying vec3 color;
//
//const vec4 lightlocation = vec4(0, 10, 0, 1);
//const vec3 Kd = vec3(1, 1, 0);
//const vec3 Id = vec3(1, 1, 1);
//const vec3 Ka = vec3(.2, .2, 0);
//const vec3 Ia = vec3(1, 1, 1);
//const vec3 Ks = vec3(.3, .3, .3);
//const vec3 Is = vec3(1, 1, 1);
//
//void main()
//{
//  fNormal = normalize(normalMatrix * normal);
//  vec4 pos = modelViewMatrix * vec4(position, 1.0);
//  
//  vec3 L = normalize(vec3(lightlocation - pos));
//  vec3 N = normalize(fNormal);
//  vec3 V = normalize(-pos.xyz);
//  vec3 R = normalize(reflect(-L,N));
//
//  vec3 diffuse = Kd * Id * max(dot(L, N), 0.0);
//  vec3 ambient = Ka * Ia;  
//  
//  float Phong = 3.5;
//  vec3 specular = Ka * Is * pow(max(dot(R, V), 0.0), Phong);
//
//  color = diffuse + ambient + specular;
//  fPosition = pos.xyz;
//  gl_Position = projectionMatrix * pos;
//}
