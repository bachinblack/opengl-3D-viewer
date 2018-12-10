#include "Obj.h"

Obj::Obj(const Transform& trans, const Shader &pr, const std::string& npath)
	: AItem(trans, pr), path(npath)
{
	this->setup();
}

Obj::~Obj()
{
}

void Obj::getTexture()
{
	textureID = loadBMP("C:/Users/54604/Documents/rupee.txt");
}

void Obj::setup()
{
	getTexture();
	//bool fake_texture = false;

	std::vector<GLfloat> vertice;
	std::vector<GLushort> indice;
	std::vector<GLfloat> textures;
	std::vector<GLushort> t_indice;
	std::vector<GLfloat> normals;
	std::vector<GLushort> n_indice;

	std::ifstream model;
	std::ifstream color;

	model.open(path);
	char c_line[128];
	char *tok = nullptr;

	while (model.getline(c_line, sizeof(c_line)))
	{
		tok = std::strtok(c_line, " ");
		if (tok == nullptr) { continue; }
		if (tok[0] == '#' || tok[0] == 'o') {
		 //std::cout << "comment" << std::endl;
		}
		else if (strcmp(tok, "v") == 0) {
			while ((tok = std::strtok(nullptr, " ")) != nullptr)
			{
				vertice.push_back(static_cast<GLfloat>(atof(tok)));
			}
		}
		else if (strcmp(tok, "vt") == 0) {
			while ((tok = std::strtok(nullptr, " ")) != nullptr)
			{
				//std::cout << "t: " << static_cast<GLfloat>(atof(tok)) << std::endl;
				textures.push_back(static_cast<GLfloat>(atof(tok)));
			}
		}
		else if (strcmp(tok, "vn") == 0) {
			while ((tok = std::strtok(nullptr, " ")) != nullptr)
			{
				normals.push_back(static_cast<GLfloat>(atof(tok)));
			}
		}
		else if (strcmp(tok, "f") == 0) {
			while ((tok = std::strtok(nullptr, " ")) != nullptr)
			{
				indice.push_back(static_cast<GLushort>(atoi(tok) - 1));
				while (*(tok - 1) != '/') { ++tok; }
				//std::cout << "i: " << static_cast<GLushort>(atoi(tok) - 1) << std::endl;
				t_indice.push_back(static_cast<GLushort>(atoi(tok) - 1));
				while (*(tok - 1) != '/') { ++tok; }
				n_indice.push_back(static_cast<GLushort>(atoi(tok) - 1));
			}
		}
	}

	//if (textures.size() == 0)
	//{
	//	fake_texture = true;
	//	for (unsigned short i = 0; i < vertice.size(); ++i)
	//	{
	//		if (i % 2 == 0) {
	//			textures.push_back(.3f);
	//			textures.push_back(.3f);
	//			textures.push_back(.3f);
	//		} else {
	//			textures.push_back(.7f);
	//			textures.push_back(.7f);
	//			textures.push_back(.7f);
	//		}
	//	}
	//}


	glGenVertexArrays(1, &this->vaoHandle);
	glBindVertexArray(this->vaoHandle); // init VAO

	glGenBuffers(1, &this->vbo_vertice);
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo_vertice);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertice.size(), vertice.data(), GL_STATIC_DRAW);
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
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * textures.size(), textures.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(
		1,
		2,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void *)0
	);
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &this->ibo_elements);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibo_elements);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * indice.size(), indice.data(), GL_STATIC_DRAW);

	//if (!fake_texture) {
		//glGenBuffers(1, &this->ibo_texture);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibo_texture);
		//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * t_indice.size(), t_indice.data(), GL_STATIC_DRAW);
	//}

	glBindVertexArray(0);

}

void Obj::perform_draw()
{
	int size;
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);

	glDrawElements(GL_TRIANGLES, size / sizeof(GLushort), GL_UNSIGNED_SHORT, 0);
}