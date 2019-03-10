#pragma once

#include <vector>

#include "AItem.h"
#include "Visual.h"

class LightItem :
	public AItem
{
public:
	LightItem(const Transform& trans, const Shader &pr, const Visual& vis, const bool setup) : AItem(trans, pr), visual(vis) { };
	LightItem(const Transform& trans, const Shader &pr, const Visual& vis = Visual());
	~LightItem() {};

protected:

	Visual visual;

	void setup() override;
	void perform_draw() override;
	
	void setModelView(const glm::mat4& view, ShaderProgram *pr) override;

	glm::vec3 computeNormal(glm::vec3 *p);
};

