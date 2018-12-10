#version 450

in vec3 fColor; // getting color data from vertex shader
in vec3 fPosition;
in vec3 fNormal;
in vec3 color;

//layout(location = 0) out vec4 FragColor;

in vec3 LightIntensity;

void main()
{
	gl_FragColor = vec4(LightIntensity, 1.0);
}

//precision highp float;
//uniform float time;
//uniform vec2 resolution;
//varying vec3 fPosition;
//varying vec3 fNormal;
//varying vec3 color;
//
//void main()
//{
//  gl_FragColor = vec4(color, 1.0);
//}