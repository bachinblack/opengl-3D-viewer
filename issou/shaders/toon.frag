#version 450

in vec3 fPosition; // position from vertex shader
in vec3 fNormal; // normal from vertex shader


// the position and color for 5 different light sources
uniform vec4 lightPos[5];
uniform vec3 lightCol[5];

uniform vec3 Kd; // diffuse object color
uniform vec3 Ka; // ambient object color
uniform vec3 La; // ambient light color

uniform float Alpha; // alpha channel (transparency)

out vec4 fragColor;

// values for rounding (levels = 3 : values = 0, .25, .5, .75)
const int levels = 3;
const float scaleFactor = 1.0 / levels;

void main()
{

	vec3 N = normalize(fNormal);

	vec3 ambient = Ka * La;

	vec3 tdiff = vec3(0, 0, 0);

//	for(int i=0;i<5;i++){
		vec3 L = normalize(vec3(lightPos[0]) - fPosition);
		float cosine = dot(L, N);  //This value has (0~1)
		float value =  floor(cosine * levels) * scaleFactor;
		vec3 diffuse = Kd * lightCol[0] * value;
		tdiff = tdiff + diffuse;
//	}

	vec3 fColor = tdiff + ambient;
	fragColor = vec4(fColor, Alpha);
}