#version 450

in vec3 fPosition; // position from vertex shader
in vec3 fNormal; // normal from vertex shader


uniform sampler2D shadow;

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


const vec3 white = vec3(1, 1, 1);
const int levels = 6;
const float scaleFactor = 1.0 / levels;


vec3 getLight()
{
	vec3 N = fNormal;
	vec3 L = normalize(vec3(lightPos[0]) - fPosition);


	float cosine = dot(L, N);  //This value has (0~1)
	float value =  floor(cosine * levels) * scaleFactor;
	
	vec3 diffuse = white * value;

	return (diffuse);
}

void main()
{
	vec2 coord = vec2(gl_FragCoord);
	coord.x = mod(coord.x, 127);
	coord.y = mod(coord.y, 127);
//	vec4 shcolor = texture2D(shadow, gl_TexCoord[0].st);
	vec4 shcolor = texture(shadow,  vec2(gl_FragCoord));
//	half intensity = dot(getLight(), half3(0.2326, 0.7152, 0.0722));
	fragColor = vec4(getLight(), Alpha);
	fragColor = shcolor;
}