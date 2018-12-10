#include "LineSegment.h"



LineSegment::LineSegment(const Transform& trans, const Shader &pr, const glm::vec4& st, const glm::vec4& en)
	: AItem(trans, pr), start(st), end(en)
{
	setup();
}


LineSegment::~LineSegment() { }

void LineSegment::setup()
{
	GLfloat	vertice[8] = {
		start.x, start.y, start.z, start.w,
		end.x  , end.y  , end.z, end.w
	};

	glm::vec3	colors[6] = {
		color,
		color
	};

	glGenVertexArrays(1, &this->vaoHandle);
	glBindVertexArray(this->vaoHandle); // init VAO

	glGenBuffers(1, &this->vbo_vertice);
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo_vertice);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertice), vertice, GL_STATIC_DRAW);
	glVertexAttribPointer(
		0,
		4,
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
	glBindVertexArray(0);
}

void LineSegment::update()
{
	GLfloat	vertice[8] = {
		start.x, start.y, start.z, start.w,
		end.x  , end.y  , end.z, end.w
	};
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo_vertice);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertice), vertice, GL_STATIC_DRAW);
}

void LineSegment::perform_draw()
{
	glLineWidth(3.0);

	glDrawArrays(GL_LINES, 0, 2);
}

void LineSegment::setPoints(const glm::vec4& st, const glm::vec4& en) {
	start = st; end = en;
	//setup();
	update();
}

void LineSegment::setColor(const glm::vec3& col) { color = col; }
