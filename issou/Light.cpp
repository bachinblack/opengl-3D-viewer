#include "Light.h"



Light::Light(const Transform& trans, const Shader &pr)
	: AItem(trans, pr)
{
	this->setup();
}


void Light::setup()
{
	GLfloat vertices[] = {
		// base
		-0.5, .5, .5,
		-.5, .5, -0.5,
		.5, .5 , 0,
		 // bottom
		0, -0.5, 0,
	};

	//GLushort data[] = {
	//	0, 1, 2,
	//	1, 2, 3,
	//	2, 0, 3,
	//	0, 1, 3
	//};

	GLushort data[] = {
		0, 1,
		1, 2,
		2, 0,
		0, 3,
		1, 3,
		2, 3,
	};

	GLfloat colors[] = {
		.9f, .35f, .2f,
		.9f, .35f, .2f,
		.9f, .35f, .2f,
		.8f, .25f, .8f,
	};

	glGenVertexArrays(1, &this->vaoHandle);
	glBindVertexArray(this->vaoHandle); // init VAO

	glGenBuffers(1, &this->vbo_vertice);
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo_vertice);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void *)0
	);
	glEnableVertexAttribArray(0);
	glGenBuffers(1, &this->vbo_colors);
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo_colors);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
	glVertexAttribPointer(
		1,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void *)0
	);
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &this->ibo_elements);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibo_elements);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void Light::perform_draw()
{
	glLineWidth(5.0);

	int size;
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);

	glDrawElements(GL_LINES, size / sizeof(GLushort), GL_UNSIGNED_SHORT, 0);
}