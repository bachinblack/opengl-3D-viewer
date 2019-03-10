#pragma once

// to_string()
# define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>

#include "ModelView.h"
#include "Loader.h"
#include "Transform.h"


class IItem
{
public:
	IItem() {};
	virtual ~IItem() {};

	virtual void draw(const glm::mat4&) = 0;

private:

	virtual void setup() = 0;
	virtual void perform_draw() = 0;
};

struct Shader {
	Shader(glm::mat4 * const v, glm::mat4 * const pj, ShaderProgram * const pg=nullptr)
		: view(v), projection(pj), program(pg) {};
public:
	glm::mat4		*view;
	glm::mat4		*projection;
	ShaderProgram	*program;
};

class AItem
{
public:
	AItem(const Transform& trans, const Shader &pr);
	~AItem();

	void setVisible(const bool);
	void draw(const glm::mat4&, ShaderProgram *pr = nullptr);
	
	Transform		transform;

protected:

	virtual void setup();
	virtual void perform_draw();
	
	virtual void apply_transform();
	virtual void setModelView(const glm::mat4& view, ShaderProgram *pr = nullptr);

	Shader					shader;
	Model					m_model;

	bool			visible;
	glm::mat4		model;

	GLuint	vaoHandle;
	GLuint	vbo_vertice, vbo_colors, vbo_normals;
	GLuint	ibo_elements;
};

