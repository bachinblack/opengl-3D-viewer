#version 450

in vec3 fColor; // getting color data from vertex shader
in vec3 fPosition;
in vec3 fNormal;

uniform vec4 lightLoc;
uniform vec3 Kd;
uniform vec3 Ld;
uniform vec3 Ka;
uniform vec3 La;
uniform vec3 Ks;
uniform vec3 Ls;

uniform float Shininess;
uniform float Alpha;


uniform mat3 normalMatrix;

out vec4 fragColor;

void main()
{
	vec3 L = normalize(vec3(lightLoc) - fPosition);
	vec3 N = normalize(fNormal);
	vec3 V = normalize(-fPosition);
	vec3 R = normalize(reflect(-L,N));

	vec3 diffuse = Kd * Ld * max(dot(L, N), 0.0);
	vec3 ambient = Ka * La; 

	vec3 specular = Ks * Ls * pow(max(dot(R, V), 0.0), Shininess);

	vec3 fColor = diffuse + ambient + specular;

	fragColor = vec4(fColor, Alpha);
}