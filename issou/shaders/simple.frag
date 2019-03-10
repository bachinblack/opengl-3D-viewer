#version 450

in vec3 fColor; // getting color data from vertex shader

// "out vec4" for setting the fragment colors
out vec4 fragColor;


void main()
{
	fragColor = vec4(fColor, 1.0);
}