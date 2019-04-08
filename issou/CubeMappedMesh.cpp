#include "CubeMappedMesh.h"
#include "stb/stb_image.h"

CubeMappedMesh::CubeMappedMesh(const Transform& trans, const Shader& sh, const char *filename, const Visual& vis)
	: Mesh(trans, sh, filename, vis)
{

}


CubeMappedMesh::~CubeMappedMesh() { }

void CubeMappedMesh::perform_draw()
{
	for (auto m : meshEntries) {

		glBindTexture(GL_TEXTURE_2D, m->txt);
		//glUniform1i(this->shader.program->uniform("tex"), 0);
		//glUniform1i(this->shader.program->uniform("normalTex"), 1);
		glUniform1i(this->shader.program->uniform("drawSkyBox"), GL_FALSE);
		glUniform1f(shader.program->uniform("Shininess"), (m->shininessStrength || 1.0f));

		m->render();
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}