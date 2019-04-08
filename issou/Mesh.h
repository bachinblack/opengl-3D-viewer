#pragma once

#include "GL/glew.h"
#include <GLFW/glfw3.h>

#include <vector>

#include "assimp\scene.h"
#include "assimp\mesh.h"
#include "LightItem.h"

class Mesh : public LightItem
{
public:
	Mesh(const Transform& trans, const Shader& sh, const char *filename, const Visual& vis = Visual());
	~Mesh(void);

	struct MeshEntry {
		enum BUFFERS {
			VERTEX_BUFFER, TEXCOORD_BUFFER, NORMAL_BUFFER, INDEX_BUFFER, TANGENT, BITTANGENT
		};
		GLuint vao;
		GLuint vbo[6];

		unsigned int elementCount;
		aiColor3D dcolor;
		aiColor3D acolor;
		aiColor3D scolor;
		float shininessStrength;
		MeshEntry(const unsigned short id, const aiScene* scene, Mesh * m);
		~MeshEntry();
		Mesh * parent;	
		void render();	
	
		std::string texpath, normpath;
		//char *texpath;
		unsigned int texID;
		GLuint txt;
	};


	std::vector<MeshEntry*> meshEntries;

protected:
	std::string		_root;

	void setRoot(const std::string& name);
	void perform_draw() override;
};