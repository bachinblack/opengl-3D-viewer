#version 450

uniform float Alpha; // alpha channel (transparency)


out vec4 fragColor;


void main()
{
	fragColor = vec4(0, 0, 0, Alpha);
}