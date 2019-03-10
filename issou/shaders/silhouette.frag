#version 450

in vec3 fPosition; // position from vertex shader

uniform float Alpha; // alpha channel (transparency)

out vec4 fragColor;

void main()
{
	vec3 fColor = vec3(0, 0, 0);
	fragColor = vec4(fColor, Alpha);
}