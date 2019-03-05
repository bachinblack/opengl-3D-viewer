#version 450

in vec3 fColor; // getting color data from vertex shader
in vec3 fPosition;
in vec3 fNormal;

//struct SpotLight {
//	vec4 pos;
//	vec3 intensity;
//	vec3 direction;
//	float exponent;
//	float cutoff;
//	float Innercutoff;
//};

uniform vec3 lightPos;
uniform vec3 lightDirection;
uniform vec3 lightIntensity;

uniform float cutOff;
uniform float innerCutoff;
uniform float exponent;

uniform vec3 Kd;
uniform vec3 Ka;
uniform vec3 Ks;
uniform float Shininess;

out vec3 fragColor;

const float a = 1.0f;
const float b = 0.01f;
const float c = 0.001f;

vec3 calculateSpot()
{
	vec3 color;
	vec3 L = normalize(lightPos.xyz - fPosition); // Light vector
	float d = length(L); // attenuation
	float attenuation = 1.0f / (a + b*d + c*d*d);
	
	float spotDir = dot(-L, normalize(lightDirection));
	float angle = dot(-L, normalize(lightPos)); // rad angle
	
	float spotAttenuation;
	if (angle < radians(innerCutoff)) {
		// position is in the inner cone
		spotAttenuation = 1.0;
	} else {
		float spotValue = smoothstep(cos(radians(cutOff)), cos(radians(innerCutoff)), spotDir);
		spotAttenuation = pow(spotValue, exponent);
	}
	attenuation *= spotAttenuation;
	// blinn-phong specular
	vec3 V = normalize(-fPosition);
	vec3 H = normalize(V+L);
	// phong
	float dotNL = max(dot(fNormal, L), 0);
	float dotHN = max(dot(H, fNormal), 0);
	float pf = pow(dotHN, Shininess);
	
	vec3 ambient = Ka * lightIntensity * attenuation;
	vec3 diffuse = Kd * lightIntensity * dotNL * attenuation;
	vec3 specular = Ks * lightIntensity * pf * attenuation;
	
	return color;
}

void main()
{
	vec3 spot = calculateSpot();
	fragColor = fColor;
}