#include "TexturedItem.h"
#include "stb/stb_image.h"


TexturedItem::TexturedItem(const Transform& tr, const Shader& sh, const char *filename, const char *texturename, const Visual& vis)
	: LightItem(tr, sh, vis), _txtname(texturename)
{
	glBindVertexArray(this->vaoHandle); // init VAO

	int width = 0, height = 0, channel = 0;

	glEnable(GL_TEXTURE_2D);
	glGenTextures(2, _txt);


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _txt[0]);

	unsigned char *image = stbi_load(_txtname, &width, &height, &channel, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	
	//glActiveTexture(GL_TEXTURE1);
	//glBindTexture(GL_TEXTURE_2D, _txt[1]);

	//image = stbi_load("./resources/moss.png", &width, &height, &channel, 0);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	//glGenerateMipmap(GL_TEXTURE_2D);

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	GLfloat textures[36 * 2] = {
		// one triangle 1
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		//the other triangle
		0.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		// one triangle 2
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		//the other triangle
		0.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		// one triangle 3
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		//the other triangle
		0.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		// one triangle 4
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		//the other triangle
		0.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		// one triangle 5
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		//the other triangle
		0.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		// one triangle 6
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		//the other triangle
		0.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
	};

	glGenBuffers(1, &this->_vbotxt);
	glBindBuffer(GL_ARRAY_BUFFER, this->_vbotxt);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 36 * 2, textures, GL_STATIC_DRAW);
	glVertexAttribPointer(
		2,
		2,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void *)0
	);
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
}

void TexturedItem::perform_draw()
{
	glUniform1i(this->shader.program->uniform("text"), 0);
	//glUniform1i(this->shader.program->uniform("foil"), 1);

	glBindVertexArray(vaoHandle);
	glDrawArrays(GL_TRIANGLES, 0, 36);

}
