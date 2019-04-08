#pragma once

#include "LightItem.h"
#include "Sphere.h"

class TexturedSphere :
	public Sphere
{
public:
	TexturedSphere(const Transform& tr, const Shader& sh, const char *texturename, const Visual& vis);
	~TexturedSphere() {};
private:
	GLuint				_txt;
	GLuint				_vbotxt;
	const char		   *_txtname;

	void perform_draw() override;
};

