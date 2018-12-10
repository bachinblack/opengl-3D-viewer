#include "CheckeredFloor.h"
#include <vector>

int nbSquares = 0;


CheckeredFloor::CheckeredFloor(const Transform& trans, const Shader &pr, const int s, const int ns)
	: AItem(trans, pr), size(s), nsquares(ns)
{
	nbSquares = nsquares;
	setup();
}


CheckeredFloor::~CheckeredFloor()
{
}

void CheckeredFloor::setup()
{
	float usize = static_cast<float>(size) / nsquares;

	std::vector<glm::vec3>	vertices;
	std::vector<glm::vec3>	colors;
	//std::vector<glm::vec3>	indice;
	
	glm::vec3 tmp(0, 0, 0);
	glm::vec3 color1(.7, .7, .7);
	glm::vec3 color2(.3, .3, .3);

	for (short i = 0; i < nsquares; ++i) {
		for (short j = 0; j < nsquares; ++j) {
		// vertices
			glm::vec3 cur(tmp);
			vertices.push_back(cur);
			cur.x += usize;
			vertices.push_back(cur);
			cur.x -= usize;
			cur.z += usize;
			vertices.push_back(cur);
			vertices.push_back(cur);
			cur.x += usize;
			vertices.push_back(cur);
			cur.z -= usize;
			vertices.push_back(cur);
			tmp.x += usize;
		// color
			colors.push_back((i + j) % 2 == 0 ? color1 : color2);
			colors.push_back((i + j) % 2 == 0 ? color1 : color2);
			colors.push_back((i + j) % 2 == 0 ? color1 : color2);
			colors.push_back((i + j) % 2 == 0 ? color1 : color2);
			colors.push_back((i + j) % 2 == 0 ? color1 : color2);
			colors.push_back((i + j) % 2 == 0 ? color1 : color2);
		// indice
			//indice.push_back(index)
		}
		tmp.x = 0;
		tmp.z += usize;
	}

	std::cout << vertices.size() << std::endl;
	//for (glm::vec3 tmp : vertices) {
	//	std::cout << tmp << std::endl;
	//}

	//GLfloat cube_vertices[] = {
	//	// front
	//	-0.5 , -0.5 , 0.5 ,
	//	0.5 , -0.5 , 0.5 ,
	//	0.5 , 0.5 , 0.5 ,
	//	-0.5 , 0.5 , 0.5 ,
	//	// back
	//	-0.5 , -0.5 , -0.5,
	//	0.5 , -0.5 , -0.5 ,
	//	0.5 , 0.5 , -0.5 ,
	//	-0.5 , 0.5 , -0.5 ,
	//};

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

	//GLushort cube_data[] = {
	//	0, 1, 2,	2, 3, 1,
	//	1, 4, 3,	3, 5, 4,
	//	4, 6, 5,	5, 7, 6,
	//};


	glGenVertexArrays(1, &this->vaoHandle);
	glBindVertexArray(this->vaoHandle); // init VAO

	glGenBuffers(1, &this->vbo_vertice);
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo_vertice);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertices.size() * 3, vertices.data(), GL_STATIC_DRAW);
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
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*colors.size() * 3, colors.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(
		1,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void *)0
	);
	glEnableVertexAttribArray(1);

	//glGenBuffers(1, &this->ibo_elements);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibo_elements);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_data), cube_data, GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void CheckeredFloor::perform_draw()
{
	glDrawArrays(GL_TRIANGLES, 0, nbSquares*nbSquares*2*3);
}