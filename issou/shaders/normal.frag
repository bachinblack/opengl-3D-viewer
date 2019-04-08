#version 450

in vec3 fPosition; // position from vertex shader
in vec3 fNormal; // normal from vertex shader
in vec2 fUV;
in mat3 tanMat;

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

uniform sampler2D tex;
uniform sampler2D normalTex;


out vec4 fragColor;


void main()
{
//	vec3 N = normalize(fNormal);
	vec3 newN = vec3(texture(normalTex, fUV));
	
//	vec3 V = normalize(-fPosition);
	vec3 newV = normalize(tanMat *(-fPosition));
	vec3 ambient = Ka * La;

	vec3 diffspec = vec3(0, 0, 0);

	for(int i=0;i<2;i++){
//		vec3 L = normalize(vec3(lightPos[i]) - fPosition);
		vec3 newL = normalize(tanMat * (vec3(lightPos[i]) - fPosition));
		vec3 R = normalize(reflect(-newL,newN));
		vec3 diffuse = Kd * lightCol[i] * max(dot(newL, newN), 0.0);
		vec3 specular = Ks * lightCol[i] * pow(max(dot(R, newV), 0.0), Shininess);
		diffspec = diffspec + diffuse + specular;
	}


	vec3 fColor = ambient + diffspec;
	vec4 textCol = texture(tex, fUV);
	
	fragColor = vec4(fColor, Alpha) * textCol;
}