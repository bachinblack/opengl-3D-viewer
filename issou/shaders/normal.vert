#version 450

layout (location  = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 vertexUV;
layout (location = 4) in vec3 tangent;
layout (location = 5) in vec3 bittangent;


uniform mat4 modelView;
uniform mat4 projection;
uniform mat3 normalMatrix;


out vec2 fUV;
out vec3 fPosition;
out vec3 fNormal;
out mat3 tanMat;


mat3 getTangentMat()
{
	vec3 norm = normalize(normalMatrix * normal );
	vec3 tang = normalize(normalMatrix * tangent);
	vec3 bittang = normalize(normalMatrix * bittangent);
	
	mat3 tmat = mat3(
		tang.x, norm.x, bittang.x,
		tang.y, norm.y, bittang.y,
		tang.z, norm.z, bittang.z
	);

	return tmat;
}


void main()
{
	
	tanMat = getTangentMat();
	
	fUV = vertexUV;

	fNormal = normalize(normalMatrix * normal);

	vec4 pos = modelView * vec4(vertexPosition, 1.0);
	fPosition = pos.xyz;
	
	gl_Position = projection * pos;
}