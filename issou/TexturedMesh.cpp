#include "TexturedMesh.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"



TexturedMesh::TexturedMesh(const Transform& trans, const Shader& sh, const char *filename, const Visual& vis)
	: Mesh(trans, sh, filename, vis)
{
	std::cout << "Meshes number: " << meshEntries.size() << std::endl;
	for (MeshEntry *it : meshEntries) {
		if (it->texpath != "" && it->texID >= 0) {
			glBindVertexArray(it->vao); // init VAO
			glActiveTexture(GL_TEXTURE0);


			glGenTextures(1, &it->txt);
			glBindTexture(GL_TEXTURE_2D, it->txt);

			TextureManager::Texture tex = _textures.getTexData(it->texID, (_root + it->texpath).c_str());
			if (tex.data) {
				if (tex.channel == 4) {
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex.width, tex.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex.data);
				}
				else {
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex.width, tex.height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex.data);
				}
			}

			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		
			/////////////////
			//   normal    //
			/////////////////
			//glActiveTexture(GL_TEXTURE1);

			//glGenTextures(1, &_normal);
			//glBindTexture(GL_TEXTURE_2D, _normal);

			//std::cout << textname << std::endl;
			//img = stbi_load((_root + it->normpath).c_str(), &width, &height, &channel, 0);
			//if (img) {
			//	if (channel == 4) {
			//		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);
			//	}
			//	else {
			//		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
			//	}
			//}
			//else {
			//	std::cerr << "failed to open: " + _root + it->normpath + "." << std::endl;
			//}
			//glGenerateMipmap(GL_TEXTURE_2D);

			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

			//glActiveTexture(GL_TEXTURE0);
			glBindVertexArray(0);
		}
	}
}


TexturedMesh::~TexturedMesh()
{
}

void TexturedMesh::perform_draw()
{
	for (auto m : meshEntries) {
		
		glBindTexture(GL_TEXTURE_2D, m->txt);
		glUniform1i(this->shader.program->uniform("tex"), 0);
		//glUniform1i(this->shader.program->uniform("normalTex"), 1);
		
		glUniform1f(shader.program->uniform("Shininess"), (m->shininessStrength || 1.0f));

		m->render();
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}


/////////////////////////
//   Texture Manager   //
/////////////////////////

TextureManager::TextureManager()
{
	_default.id = 0;

	unsigned char *img = stbi_load("./resources/default.jpg", &_default.width, &_default.height, &_default.channel, 0);
	if (img) {
		_default.data = img;
	}
	else {
		std::cerr << "Couldn't find [./resources/default.jpg]. No default texture available." << std::endl;
	}
}

TextureManager::~TextureManager()
{
	for (auto it : _tex) {
		stbi_image_free(it.data);
	}
}


TextureManager::Texture TextureManager::getTexData(const unsigned int id, const char *name)
{
	// checking if the texture is already registered
	for (auto it : _tex) {
		if (it.id == id) {
			return it;
		}
	}

	// if not, creating it
	Texture t;
	
	t.id = id;
	t.data = nullptr;

	std::cout << "[TextureManager] " << id << ": "<< name << std::endl;
	unsigned char *img = stbi_load(name, &t.width, &t.height, &t.channel, 0);
	if (img) {
		t.data = img;
		_tex.push_back(t);
	}
	else {

		std::cerr << "[TextureManager] Couldn't find [" << name << "]. Using default texture." << std::endl;
		return _default;
	}

	return t;
}