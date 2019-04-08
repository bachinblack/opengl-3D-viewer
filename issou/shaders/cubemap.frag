#version 450

in vec3 reflectDir;
in vec3 refractDirR;
in vec3 refractDirG;
in vec3 refractDirB;
in float reflectFactor;

uniform float Alpha; // alpha channel (transparency)
uniform samplerCube cubeMapTex;
uniform bool drawSkyBox;


out vec4 fragColor;


const vec4 matColor = vec4(.4, .4, .4, 1);
//const float reflectFactor = .85;
const float refractFactor = .1;


void main()
{
	vec4 reflectColor = texture(cubeMapTex, reflectDir);
	vec4 refractColor;
	refractColor.r = texture(cubeMapTex, refractDirR).r;
	refractColor.g = texture(cubeMapTex, refractDirG).g;
	refractColor.b = texture(cubeMapTex, refractDirB).b;
	
	if (drawSkyBox) {
		fragColor = vec4(1, 1, 1, Alpha) * reflectColor;
	} else {
		vec4 reflectmix = mix(matColor, reflectColor, reflectFactor);
		fragColor = mix(refractColor, reflectmix, refractFactor);
		//fragColor = mix(matColor, reflectColor, reflectFactor);
	
	}
	
}