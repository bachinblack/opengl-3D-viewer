#version 450

in vec3 fPosition; // position from vertex shader
in vec3 fNormal; // normal from vertex shader


uniform vec4 lightPos[6];
uniform vec3 lightCol[6];

uniform vec3 Kd; // diffuse object color
uniform vec3 Ka; // ambient object color
uniform vec3 La; // ambient light color
uniform vec3 Ks; // specular object color
uniform vec3 Ls; // specular light color

uniform float Shininess; // shininess of the object
uniform float Alpha; // alpha channel (transparency)


out vec4 fragColor;

const float maxDist = 30.0;
const float minDist = 5;
const vec3 col = vec3(.6, .6, .6);

float getDistance()
{
	float z = length(fPosition);

	float f = (maxDist - abs(z)) / (maxDist - minDist);

	f = clamp(f, 0.0, 1.0);
	
	return f;
}

vec3 getLight()
{
	vec3 L = normalize(vec3(lightPos[5]) - fPosition);
	vec3 N = normalize(fNormal);
	vec3 V = normalize(-fPosition);
	vec3 R = normalize(reflect(-L,N));

	vec3 diffuse = Kd * lightCol[5] * max(dot(L, N), 0.0);
	vec3 ambient = Ka * La; 

	vec3 specular = Ks * Ls * pow(max(dot(R, V), 0.0), Shininess);

	return (diffuse + ambient + specular);
}

void main()
{
	vec3 color = mix(vec3(.5, .5, .5), getLight(), getDistance());

	fragColor = vec4(color, Alpha);
}