#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <cstring>
#include <string>
#include <vector>
#include <map>
#include <functional>

#include "AItem.h"
#include "Medias.h"

class Obj : public AItem
{
public:
	Obj(const Transform& trans, const Shader &pr, const std::string& npath);
	~Obj();

private:

	void setup() override;
	void perform_draw() override;

	void getTexture();

	const std::string	path;
	GLuint				textureID;
	GLuint				ibo_texture;
};

