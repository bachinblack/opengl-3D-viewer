#pragma once

#include "Mesh.h"


////////////////////////
//
// The goal of the TextureManager is to keep track of every textures already loaded to avoid loading duplication
//

class TextureManager
{
public:
	struct Texture {
		unsigned int id;
		int width, height, channel;
		unsigned char *data;
	};

	TextureManager();
	~TextureManager();

	Texture getTexData(const unsigned int id, const char *name);

private:
	Texture				 _default;
	std::vector<Texture> _tex;
};


class TexturedMesh :
	public Mesh
{
public:
	TexturedMesh(const Transform& trans, const Shader& sh, const char *filename, const Visual& vis = Visual());
	~TexturedMesh();
private:

	TextureManager			_textures;
	GLuint					_txt;
	GLuint					_normal;

	void perform_draw() override;
};
