#pragma once

#include "GL/glew.h"
#include <GLFW/glfw3.h>

#include <vector>

#include "assimp\scene.h"
#include "assimp\mesh.h"
#include "LightItem.h"

# define MODEL_PATH ""

class Mesh : public LightItem
{
public:
	Mesh(const Transform& trans, const Shader& sh, const char *filename, const Visual& vis = Visual());
	~Mesh(void);

	struct MeshEntry {
		enum BUFFERS {
			VERTEX_BUFFER, TEXCOORD_BUFFER, NORMAL_BUFFER, INDEX_BUFFER
		};
		GLuint vao;
		GLuint vbo[4];

		unsigned int elementCount;
		aiColor3D dcolor;
		aiColor3D acolor;
		aiColor3D scolor;
		float shininessStrength;
		MeshEntry(aiMesh *mesh, const aiScene* scene, Mesh * m);
		~MeshEntry();
		Mesh * parent;	
		void render();	
	};
		
	std::vector<MeshEntry*> meshEntries;

private:
	void perform_draw() override;	
};