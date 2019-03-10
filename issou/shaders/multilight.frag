#version 450

in vec3 fPosition; // position from vertex shader
in vec3 fNormal; // normal from vertex shader


// the position and color for 5 different light sources
uniform vec4 lightPos[5];
uniform vec3 lightCol[5];

uniform vec3 Kd; // diffuse object color
uniform vec3 Ka; // ambient object color
uniform vec3 La; // ambient light color
uniform vec3 Ks; // specular object color
uniform vec3 Ls; // specular light color

uniform float Shininess; // shininess of the object
uniform float Alpha; // alpha channel (transparency)


out vec4 fragColor;


void main()
{
	vec3 N = normalize(fNormal);
	vec3 V = normalize(-fPosition);

	vec3 ambient = Ka * La;

	vec3 diffspec = vec3(0, 0, 0);

	for(int i=0;i<5;i++){
		vec3 L = normalize(vec3(lightPos[i]) - fPosition);
		vec3 R = normalize(reflect(-L,N));
		vec3 diffuse = Kd * lightCol[i] * max(dot(L, N), 0.0);
		vec3 specular = Ks * Ls * pow(max(dot(R, V), 0.0), Shininess);
		diffspec = diffspec + diffuse + specular;
	}

	vec3 fColor = diffspec + ambient;
	fragColor = vec4(fColor, Alpha);
}