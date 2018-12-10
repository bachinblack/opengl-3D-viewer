#ifndef ___SPHERE_H
#define ___SPHERE_H

#include "GL/glew.h"
#include <GL/GL.h>
#include <glm/mat4x4.hpp>

#include <vector>

#include "LightItem.h"

class Sphere : public LightItem
{
public:
	Sphere(
		const Transform& tr, const Shader& pr,
		const float rad, const GLuint sl, const GLuint st, const Visual& vis = Visual()
	);
	~Sphere() {};

	int getVertexArrayHandle();

private:

	void setup() override;
	void perform_draw() override;

	GLuint nVerts, elements;
	float radius;
	GLuint slices, stacks;

	void generateVerts(float *, float *, float *, GLuint *);
};

#endif
