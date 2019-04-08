#pragma once
#include "Mesh.h"
#include "LightItem.h"

class TexturedItem :
	public LightItem
{
public:
	TexturedItem(const Transform& trans, const Shader& sh, const char *filename, const char *texturename = nullptr, const Visual& vis = Visual());
	~TexturedItem() {};

private:
	GLuint				_txt[2];
	GLuint				_vbotxt;
	const char		   *_txtname;
	//static int texturesNb = 0;

	void perform_draw() override;
};

