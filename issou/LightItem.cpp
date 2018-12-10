#include "LightItem.h"


LightItem::LightItem(const Transform& trans, const Shader &pr, const Visual& vis)
	: AItem(trans, pr), visual(vis)
{
	this->setup();
}

glm::vec3 LightItem::computeNormal(glm::vec3 *p)
{
	glm::vec3 U = p[1] - p[0];
	glm::vec3 V = p[2] - p[0];
	
	glm::vec3 normal = glm::cross(U, V);



	return normal;
}

void LightItem::setup()
{
	GLfloat colors[] = {
		0.0, 1.0, 0.0,	// 5
		1.0, 0.0, 0.0,	// 1
		1.0, 0.0, 0.0,	// 4
		
		0.0, 1.0, 0.0,	// 5
		1.0, 0.0, 0.0,	// 4
		0.0, 1.0, 0.0,	// 8

		0.0, 0.0, 1.0,	// 3
		0.0, 0.0, 1.0,	// 7
		0.0, 1.0, 0.0,	// 8

		0.0, 0.0, 1.0,	// 3
		0.0, 1.0, 0.0,	// 8
		1.0, 0.0, 0.0,	// 4

		0.0, 1.0, 0.0,	// 2
		0.0, 0.0, 1.0,	// 6
		0.0, 0.0, 1.0,	// 3

		0.0, 0.0, 1.0,	// 6
		0.0, 0.0, 1.0,	// 7
		0.0, 0.0, 1.0,	// 3

		1.0, 0.0, 0.0,	// 1
		0.0, 1.0, 0.0,	// 5
		0.0, 1.0, 0.0,	// 2
	
		0.0, 1.0, 0.0,	// 5
		0.0, 0.0, 1.0,	// 6
		0.0, 1.0, 0.0,	// 2
	
		0.0, 1.0, 0.0,	// 5
		0.0, 1.0, 0.0,	// 8
		0.0, 0.0, 1.0,	// 6

		0.0, 1.0, 0.0,	// 8
		0.0, 0.0, 1.0,	// 7
		0.0, 0.0, 1.0,	// 6

		1.0, 0.0, 0.0,	// 1
		0.0, 1.0, 0.0,	// 2
		0.0, 0.0, 1.0,	// 3

		1.0, 0.0, 0.0,	// 1
		0.0, 0.0, 1.0,	// 3
		1.0, 0.0, 0.0,	// 4
	};
	
	GLfloat vertice[] = {

		1, 1, -1, // 1
		1, -1, -1,
		-1, -1, -1,

		1, 1, -1, // 1
		-1, -1, -1,
		-1, 1, -1,

		-1, -1, 1, // 2
		-1, 1, 1,
		-1, 1, -1,

		-1, -1, 1, // 2
		-1, 1, -1,
		-1, -1, -1,

		1, -1, 1, // 3
		1, 1, 1,
		-1, -1, 1,

		1, 1, 1, // 3
		-1, 1, 1,
		-1, -1, 1,

		1, -1, -1, // 4
		1, 1, -1,
		1, -1, 1,

		1, 1, -1, // 4
		1, 1, 1,
		1, -1, 1,

		1, 1, -1, // 5
		-1, 1, -1,
		1, 1, 1,
		
		-1, 1, -1, // 5
		-1, 1, 1,
		1, 1, 1,

		1, -1, -1,
		1, -1, 1,
		-1, -1, 1,

		1, -1, -1,
		-1, -1, 1,
		-1, -1, -1
	};

	std::vector<glm::vec3> normals;

	glm::vec3 tmp[3];
	glm::vec3 normal;
	unsigned short size = sizeof(vertice) / sizeof(GLfloat);
	for (unsigned short i = 0; i < size; i += 9)
	{
		tmp[0] = glm::vec3(vertice[i], vertice[i + 1], vertice[i + 2]);
		tmp[1] = glm::vec3(vertice[i + 3], vertice[i + 4], vertice[i + 5]);
		tmp[2] = glm::vec3(vertice[i + 6], vertice[i + 7], vertice[i + 8]);
		normal = computeNormal(tmp);
		normals.push_back(normal);
		normals.push_back(normal);
		normals.push_back(normal);
	}

	glGenVertexArrays(1, &this->vaoHandle);
	glBindVertexArray(this->vaoHandle); // init VAO

	glGenBuffers(1, &this->vbo_vertice);
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo_vertice);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertice), vertice, GL_STATIC_DRAW);
	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void *)0
	);
	glEnableVertexAttribArray(0);
	//glGenBuffers(1, &this->vbo_colors);
	//glBindBuffer(GL_ARRAY_BUFFER, this->vbo_colors);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
	//glVertexAttribPointer(
	//	2,
	//	3,
	//	GL_FLOAT,
	//	GL_FALSE,
	//	0,
	//	(void *)0
	//);
	//glEnableVertexAttribArray(2);

	glGenBuffers(1, &this->vbo_normals);
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo_normals);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals.size(), normals.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(
		1,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void *)0
	);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}

void LightItem::setModelView(const glm::mat4& view)
{
	model = m_model.getMatrix();

	glm::mat4 MV = *this->shader.view * this->model;
	glUniformMatrix4fv(shader.program->uniform("modelView"), 1, GL_FALSE, glm::value_ptr(MV));
	glUniformMatrix4fv(shader.program->uniform("projection"), 1, GL_FALSE, glm::value_ptr(*this->shader.projection));

	glm::mat4 modelview = view * model;
	glm::mat4 inverseModelView = glm::inverse(modelview);
	glm::mat3 normalMatrix = glm::mat3(glm::transpose(inverseModelView));

	glUniformMatrix3fv(shader.program->uniform("normalMatrix"), 1, GL_FALSE, glm::value_ptr(normalMatrix));
	//std::cout << "col: " << glm::to_string(visual.getColor()) << std::endl;
	glUniform3fv(shader.program->uniform("Kd"), 1, glm::value_ptr(visual.getColor()));
	glUniform1f(shader.program->uniform("Shininess"), visual.getShininess());
	glUniform1f(shader.program->uniform("Alpha"), visual.getAlpha());
}


void LightItem::perform_draw()
{
	//glm::mat4 modelview = view * model;
	//glm::mat4 inverseModelView = glm::inverse(modelview);
	//glm::mat3 normalMatrix = glm::mat3(glm::transpose(inverseModelView));
	//glUniformMatrix4fv(m_shaderProgram->uniform("normalMatrix"), 1, GL_FALSE, glm::value_ptr(normalMatrix));

	glDrawArrays(GL_TRIANGLES, 0, 36);
}