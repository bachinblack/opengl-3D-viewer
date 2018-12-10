#include "wireCube.h"



wireCube::wireCube(const Transform& trans, const Shader &pr)
	: AItem(trans, pr)
{
	this->setup();
}


wireCube::~wireCube()
{
}

void wireCube::setup()
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

	// removed duplicated lines
	GLushort wire_data[] = {
		0, 1,	1, 2,	2, 0,	2, 3,	3, 0,	1, 5,	5, 6,	6, 1,
		6, 2,	7, 6,	5, 7,	5, 4,	4, 7,
		4, 0,	3, 4,	3, 7,	1, 4,
		6, 3,
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

	glGenBuffers(1, &this->ibo_elements);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibo_elements);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(wire_data), wire_data, GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void wireCube::perform_draw()
{
	int size;
	glLineWidth(2.0);
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);

	glDrawElements(GL_LINES, size / sizeof(GLushort), GL_UNSIGNED_SHORT, 0);
}