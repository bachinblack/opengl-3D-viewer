#version 450

in vec3 fPosition; // position from vertex shader
in vec3 fNormal; // normal from vertex shader
in vec2 fUV;


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

uniform sampler2D shadow;
//uniform sampler2D shad1;
//uniform sampler2D shad2;
//uniform sampler2D shad3;
//uniform sampler2D shad4;
//uniform sampler2D shad5;
//uniform sampler2D shad6;


out vec4 fragColor;


const int levels = 6;
const float scaleFactor = 1.0 / levels;


void main()
{
	vec3 N = normalize(fNormal);
	vec3 V = normalize(-fPosition);

	vec3 ambient = Ka * La;

	vec3 tdiff = vec3(0, 0, 0);


	for(int i=0;i<5;i++){
		vec3 L = normalize(vec3(lightPos[i]) - fPosition);
		float cosine = dot(L, N);  //This value has (0~1)
		float value =  floor(cosine * levels) * scaleFactor;
		vec3 diffuse = Kd * lightCol[i] * value;
		tdiff = tdiff + diffuse;
	}

	vec3 fColor = ambient + tdiff;
	vec4 textCol = texture(shadow, fUV);
	
//	fragColor = vec4(fColor, Alpha);
//	fragColor = vec4(fUV, 1, 1);
	fragColor = vec4(vec3(textCol), 1);
}