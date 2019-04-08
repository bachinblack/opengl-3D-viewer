#pragma once
#include "LightItem.h"
class Skybox :
	public LightItem
{
public:
	Skybox(const Transform& trans, const Shader& sh, const Visual& vis = Visual());
	~Skybox();

private:
	GLuint _cmap;
	void perform_draw() override;
};

