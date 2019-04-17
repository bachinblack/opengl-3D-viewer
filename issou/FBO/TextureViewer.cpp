#include "TextureViewer.h"


TextureViewer::TextureViewer()
	: subroutine("")
{
	this->s = new ShaderProgram();
	this->vs = "./postshaders/default.vert";
	this->fs = "./postshaders/default.frag";


	this->depth = false;
	setUpShaders();
}


TextureViewer::TextureViewer(GLuint tex, const std::string& vs, const std::string& fs)
	: subroutine("")
{
	this->s = new ShaderProgram();
	this->texture = tex;
	this->vs = vs;
	this->fs = fs;
	this->depth = false;

	setUpShaders();

	setUpShaders();
}

TextureViewer::TextureViewer(const std::string& sr)
	: subroutine(sr)
{
	this->s = new ShaderProgram();
	this->texture = 0;
	this->vs = "./postshaders/default.vert";
	this->fs = "./postshaders/subroutine.frag";
	this->depth = false;

	setUpShaders();
	this->s->addSubroutine(GL_FRAGMENT_SHADER, sr);
}


TextureViewer::~TextureViewer()
{
	delete s;
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &VAO);
}

void TextureViewer::draw() {
	//Bind VAO
	glBindVertexArray(VAO);
	//Bind VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	s->use();

	glUniform1i(s->uniform("depth"), this->depth);
	if (subroutine != "") {
		GLuint index = s->subroutine(subroutine);
		glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &index);
	}

	//Bind texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->texture);

	//Draw
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	//Unbind texture for sure
	glBindTexture(GL_TEXTURE_2D, 0);

	s->disable();

	//Unbind VBO - just for sure
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//Unbind VAO
	glBindVertexArray(0);
	
}

void TextureViewer::setDepthOnly(bool depth) {
	this->depth = depth;
}

//Called only once in constructor
void TextureViewer::setUpShaders() 
{
	//We need only vertex and fragment shaders
	s->initFromFiles(vs, fs);


	
	s->use();

	//Create uniforms and attributes (filled later)

	s->addUniform("tex");
	s->addUniform("depth");
	

	//Quad verticles - omitted z coord, because it will always be 1
	float pos[] = {
		-1.0, 1.0,
		1.0, 1.0,
		-1.0, -1.0,
		1.0, -1.0
	};

	float uv[] = {
		0.0, 1.0,
		1.0, 1.0,
		0.0, 0.0,
		1.0, 0.0
	};

	//Generate VAO
	glGenVertexArrays(1, &VAO);
	//Bind VAO
	glBindVertexArray(VAO);		

	//Generate VBO
	glGenBuffers(1, &VBO);
	//Bind VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//Alocate buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(pos)+sizeof(uv), NULL, GL_STATIC_DRAW);
	//Fill VBO
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(pos), pos);
	glBufferSubData(GL_ARRAY_BUFFER, 8 * sizeof(float), sizeof(uv), uv);

	//Fill attributes and uniforms
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, (sizeof(float)* 2), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, (sizeof(float)* 2), (GLvoid*)(sizeof(float)* 8));

	glUniform1i(s->uniform("tex"), 0);

	s->disable();
	//unbind buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

//Setters
void TextureViewer::setTexture(GLuint tex) {
	this->texture = tex;
}
//Getters
GLuint TextureViewer::getTexture() {
	return this->texture;
}