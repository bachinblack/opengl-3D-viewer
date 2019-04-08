#include "Skybox.h"
#include "stb/stb_image.h"


Skybox::Skybox(const Transform& trans, const Shader& sh, const Visual& vis)
	: LightItem(trans, sh, vis)
{
	this->transform.setRotation(0, 1, 1, 1);
	this->transform.setPosition(glm::vec3(0, 0, 0));
	this->transform.setScale(40);
}


Skybox::~Skybox() { }

void Skybox::perform_draw()
{
	glUniform1i(this->shader.program->uniform("drawSkyBox"), GL_TRUE);

	glDrawArrays(GL_TRIANGLES, 0, 36);
}