#pragma once

#include "AItem.h"

class SpotLight :
	public AItem
{
public:
	SpotLight(const Transform& trans, const Shader &pr, const glm::vec3& col);
	~SpotLight();

	glm::vec3	direction;
	glm::vec3	col;
	GLfloat		cutoff;
	GLfloat		attenuation;
	GLfloat		innerCutoff;

private:
	void perform_draw() override;
	void setup() override;


};

