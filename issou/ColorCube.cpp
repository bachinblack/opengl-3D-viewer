#include "ColorCube.h"


ColorCube::ColorCube(const Transform& trans, const Shader &pr)
	: AItem(trans, pr)
{
	this->setup();
}

void ColorCube::setup()
{
	GLfloat cube_vertices[] = {
		// front
		-0.5 , -0.5 , 0.5 ,
		0.5 , -0.5 , 0.5 ,
		0.5 , 0.5 , 0.5 ,
		-0.5 , 0.5 , 0.5 ,
		// back
		-0.5 , -0.5 , -0.5,
		0.5 , -0.5 , -0.5 ,
		0.5 , 0.5 , -0.5 ,
		-0.5 , 0.5 , -0.5 ,
	};

	GLfloat cube_colors[] = {
		// front colors
		1.0, 0.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 0.0, 1.0,
		1.0, 1.0, 1.0,
		// back colors
		1.0, 0.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 0.0, 1.0,
		1.0, 1.0, 1.0,
	};

	GLushort cube_data[] = {
		0, 1, 2,	2, 3, 0,	1, 5, 6,
		6, 2, 1,	7, 6, 5,	5, 4, 7,
		4, 0, 3,	3, 7, 4,	4, 5, 1,
		1, 0, 4,	3, 2, 6,	6, 7, 3
	};

	glGenVertexArrays(1, &this->vaoHandle);
	glBindVertexArray(this->vaoHandle); // init VAO

	glGenBuffers(1, &this->vbo_vertice);
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo_vertice);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);
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
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_colors), cube_colors, GL_STATIC_DRAW);
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
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_data), cube_data, GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void ColorCube::perform_draw()
{
	int size;
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);

	glDrawElements(GL_TRIANGLES, size / sizeof(GLushort), GL_UNSIGNED_SHORT, 0);
}