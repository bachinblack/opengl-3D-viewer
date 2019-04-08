#include "TexturedSphere.h"
#include "stb/stb_image.h"


TexturedSphere::TexturedSphere(const Transform& tr, const Shader& sh,	const char *texturename, const Visual& vis)
	: Sphere(tr, sh, 1, 64, 64, vis), _txtname(texturename)
{
	glBindVertexArray(this->vaoHandle); // init VAO

	int width = 0, height = 0, channel = 0;


	glActiveTexture(GL_TEXTURE0);

	glGenTextures(1, &_txt);
	glBindTexture(GL_TEXTURE_2D, _txt);

	unsigned char *image = stbi_load(_txtname, &width, &height, &channel, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}


void TexturedSphere::perform_draw()
{
	glBindVertexArray(vaoHandle);

	glUniform1i(this->shader.program->uniform("text"), 0);

	int size;
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	glDrawElements(GL_TRIANGLES, size / sizeof(GLushort), GL_UNSIGNED_INT, 0);
}
