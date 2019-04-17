#pragma once

////////////////////////
//
// The goal of the TextureManager is to keep track of every textures already loaded to avoid loading duplication
//

#include <iostream>
#include <vector>

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