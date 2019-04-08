#include "ShaderWrappers.h"
#include "stb/stb_image.h"

//#ifndef STB_IMAGE_IMPLEMENTATION
//	#define STB_IMAGE_IMPLEMENTATION
//	#include "stb/stb_image.h"
//#endif

AShaderWrapper::AShaderWrapper() : _sh(nullptr), _lightPos(nullptr), _lightCol(nullptr), _lightNb(0) { }


AShaderWrapper::~AShaderWrapper()
{
	for (auto it : _items) { delete it; }
	if (_sh) { delete _sh; }
}


void AShaderWrapper::draw(const glm::mat4& view, const glm::mat4& projection){
	_sh->use();
	this->setUniforms(projection);
	this->perform_draw(view);
	_sh->disable();
}


void AShaderWrapper::setUniforms(const glm::mat4& projection) { }


void AShaderWrapper::perform_draw(const glm::mat4& view) { for (AItem *it : _items) { it->draw(view); } }


void AShaderWrapper::computeLight(std::vector<Light *> lights, const glm::mat4 &view) {
	short i = 0;
	
	_lightNb = static_cast<unsigned short>(lights.size());

	if (_lightPos != nullptr) { delete _lightPos; }
	_lightPos = new glm::vec4[_lightNb];
	
	if (_lightCol != nullptr) { delete _lightCol; }
	_lightCol = new glm::vec3[_lightNb];

	for (Light *it : lights) {
		_lightPos[i] = view * glm::vec4(it->transform.getPosition(), 1);
		_lightCol[i++] = it->getColor();
	}
}

void AShaderWrapper::addItem(AItem *it) {
	_items.push_back(it);
}

void AShaderWrapper::clearItems(void) {
}

ShaderProgram *AShaderWrapper::getShaderProgram(void) const { return _sh; }

void AShaderWrapper::addItems(const std::vector<AItem *>& its) { _items.insert(_items.end(), its.begin(), its.end()); }

std::vector<AItem *> AShaderWrapper::getItems(void) const { return _items; }


/*************************/
/**      Static         **/
/*************************/


ShaderProgram *createShaderProgram(const std::string shname, const std::vector<std::string>& uniforms)
{
	ShaderProgram *shader = new ShaderProgram();

	shader->initFromFiles("./shaders/" + shname + ".vert", "./shaders/" + shname + ".frag");
	for (std::string elem : uniforms) {
		shader->addUniform(elem);
	}

	return shader;
}


/*************************/
/**       Simple        **/
/*************************/

ShaderSimple::ShaderSimple()
	: AShaderWrapper()
{
	_sh = createShaderProgram("simple", std::vector<std::string>({ "MVP" }));
}


/*************************/
/**       Phong         **/
/*************************/

ShaderPhong::ShaderPhong()
	: AShaderWrapper()
{
	// instantiating shaderProgram with uniforms
	_sh = createShaderProgram("multilight",
		std::vector<std::string>({
			// vert
			"modelView",
			"projection",
			"normalMatrix",
			// frag
			"lightPos",
			"lightCol",
			"Kd",
			"Ka",
			"La",
			"Ks",
			"Ls",
			"Shininess",
			"Alpha",
		})
	);
}

void ShaderPhong::setUniforms(const glm::mat4& projection) {
	glUniformMatrix4fv(_sh->uniform("projection"), 1, GL_FALSE, glm::value_ptr(projection));

	if (_lightNb > 0) {
		glUniform4fv(_sh->uniform("lightPos"), _lightNb, glm::value_ptr(*_lightPos));
		glUniform3fv(_sh->uniform("lightCol"), _lightNb, glm::value_ptr(*_lightCol));
	}
	glUniform3fv(_sh->uniform("Ka"), 1, glm::value_ptr(glm::vec3(.2, .2, .2)));
	glUniform3fv(_sh->uniform("La"), 1, glm::value_ptr(glm::vec3(1, 1, 1)));
}


/*************************/
/**       Toon          **/
/*************************/

ShaderToon::ShaderToon() 
	: AShaderWrapper()
{
	// instantiating shaderProgram with uniforms
	_sh = createShaderProgram("toon",
		std::vector<std::string>({
			"modelView",
			//"model",
			//"view",
			"projection",
			"Kd",
			"Ka",
			"La",
			"Alpha",
			"normalMatrix",
			"lightPos",
			"lightCol",
		})
	);
}

void ShaderToon::setUniforms(const glm::mat4& projection) {
	glUniformMatrix4fv(_sh->uniform("projection"), 1, GL_FALSE, glm::value_ptr(projection));

	if (_lightNb > 0) {
		glUniform4fv(_sh->uniform("lightPos"), _lightNb, glm::value_ptr(*_lightPos));
		glUniform3fv(_sh->uniform("lightCol"), _lightNb, glm::value_ptr(*_lightCol));
	}
	glUniform3fv(_sh->uniform("Ka"), 1, glm::value_ptr(glm::vec3(.2, .2, .2)));
	glUniform3fv(_sh->uniform("La"), 1, glm::value_ptr(glm::vec3(1, 1, 1)));
}


/*************************/
/**     Silhouette      **/
/*************************/

ShaderSilouette::ShaderSilouette()
	: AShaderWrapper(), _thickness(1.01f)
{
	// instantiating shaderProgram with uniforms
	_sh = createShaderProgram("silhouette",
		std::vector<std::string>({
			"modelView",
			"projection",
			"Alpha",
			})
		);
}

void ShaderSilouette::setUniforms(const glm::mat4& projection) {
	glUniformMatrix4fv(_sh->uniform("projection"), 1, GL_FALSE, glm::value_ptr(projection));
}

void ShaderSilouette::perform_draw(const glm::mat4& view) {
	glEnable(GL_CULL_FACE); // enable culling
	glCullFace(GL_FRONT);
	glDepthMask(GL_TRUE);
	
	for (AItem *it : _items) {
		it->transform.setScale(_thickness);
		it->draw(view, _sh);
		it->transform.setScale(1);
	}

	glDisable(GL_CULL_FACE); // disable culling
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
}


/*******************/
/**      Fog      **/
/*******************/

ShaderFog::ShaderFog()
	: AShaderWrapper()
{
	// instantiating shaderProgram with uniforms
	_sh = createShaderProgram("fog",
		std::vector<std::string>({
			// vert
			"modelView",
			"projection",
			"normalMatrix",
			// frag
			"lightPos",
			"lightCol",
			"Kd",
			"Ka",
			"La",
			"Ks",
			"Ls",
			"Shininess",
			"Alpha",
		})
	);
}

void ShaderFog::setUniforms(const glm::mat4& projection) {
	glUniformMatrix4fv(_sh->uniform("projection"), 1, GL_FALSE, glm::value_ptr(projection));

	if (_lightNb > 0) {
		glUniform4fv(_sh->uniform("lightPos"), _lightNb, glm::value_ptr(*_lightPos));
		glUniform3fv(_sh->uniform("lightCol"), _lightNb, glm::value_ptr(*_lightCol));
	}
	glUniform3fv(_sh->uniform("Ka"), 1, glm::value_ptr(glm::vec3(.2, .2, .2)));
	glUniform3fv(_sh->uniform("La"), 1, glm::value_ptr(glm::vec3(1, 1, 1)));
}


/************************/
/**     Hand Drawn     **/
/************************/

ShaderHandDrawn::ShaderHandDrawn()
	: AShaderWrapper()
{
	// Instantiating shadow textures
	int width = 127, height = 127, channel = 0;
	//for (short i = 1; i < 2; ++i) {
	//	glGenTextures(1, &_shadowLevels[i]);
	//	glBindTexture(GL_TEXTURE_2D, _shadowLevels[i]);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//	std::cout << "[ShaderHandDrawn] Adding texture:" << ("./resources/tam/tam" + std::to_string(i) + ".png") << std::endl;
	//	unsigned char *image = stbi_load(
	//		("./resources/tam/tam" + std::to_string(i) + ".bmp").c_str(),
	//		&width,
	//		&height,
	//		&channel,
	//		0
	//	);
	//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	//	//glGenerateMipmap(GL_TEXTURE_2D);
	//
	//	glActiveTexture(GL_TEXTURE0 + (i-1));
	//	glBindTexture(GL_TEXTURE_2D, _shadowLevels[i]);
	//}

	// instantiating shaderProgram with uniforms
	_sh = createShaderProgram("hand_drawn",
		std::vector<std::string>({
			// vert
			"modelView",
			"projection",
			"normalMatrix",
			// frag
			"shadow",
			"lightPos",
			"lightCol",
			"Kd",
			"Ka",
			"La",
			"Ks",
			"Ls",
			"Shininess",
			"Alpha"
		})
	);
}

void ShaderHandDrawn::setUniforms(const glm::mat4& projection) {
	
	glUniform1i(_sh->uniform("shadow"), 0);

	glUniformMatrix4fv(_sh->uniform("projection"), 1, GL_FALSE, glm::value_ptr(projection));

	if (_lightNb > 0) {
		glUniform4fv(_sh->uniform("lightPos"), _lightNb, glm::value_ptr(*_lightPos));
		glUniform3fv(_sh->uniform("lightCol"), _lightNb, glm::value_ptr(*_lightCol));
	}
	glUniform3fv(_sh->uniform("Ka"), 1, glm::value_ptr(glm::vec3(.2, .2, .2)));
	glUniform3fv(_sh->uniform("La"), 1, glm::value_ptr(glm::vec3(1, 1, 1)));
}



/*********************/
/**    Textured     **/
/*********************/

ShaderTextured::ShaderTextured()
	: AShaderWrapper()
{
	// instantiating shaderProgram with uniforms
	_sh = createShaderProgram("textured",
		std::vector<std::string>({
			// vert
			"modelView",
			"projection",
			"normalMatrix",
			// frag
			"lightPos",
			"lightCol",
			"Kd",
			"Ka",
			"La",
			"Ks",
			"Ls",
			"Shininess",
			"Alpha",
			"tex",
			//"normalTex",
		})
	);
}

void ShaderTextured::setUniforms(const glm::mat4& projection) {
	glUniformMatrix4fv(_sh->uniform("projection"), 1, GL_FALSE, glm::value_ptr(projection));

	if (_lightNb > 0) {
		glUniform4fv(_sh->uniform("lightPos"), _lightNb, glm::value_ptr(*_lightPos));
		glUniform3fv(_sh->uniform("lightCol"), _lightNb, glm::value_ptr(*_lightCol));
	}
	glUniform3fv(_sh->uniform("Ka"), 1, glm::value_ptr(glm::vec3(.2, .2, .2)));
	glUniform3fv(_sh->uniform("La"), 1, glm::value_ptr(glm::vec3(.3, .3, .3)));
}



/*********************/
/**    Cube map     **/
/*********************/

ShaderSkyBox::ShaderSkyBox(const std::string& path, glm::vec3 *camPos)
	: AShaderWrapper(), _camPos(camPos)
{
	// instantiating shaderProgram with uniforms
	_sh = createShaderProgram("cubemap",
		std::vector<std::string>({
			"drawSkyBox",
			// vert
			"model",
			"view",
			"projection",
			// frag
			"Alpha",
			"cubeMapTex",
			"camPos"
		})
	);

	setMapTextures(path);
}


void ShaderSkyBox::setUniforms(const glm::mat4& projection) {
	glUniformMatrix4fv(_sh->uniform("projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniform1i(_sh->uniform("cubeMapTex"), 0);
	glUniform3fv(_sh->uniform("camPos"), 1, glm::value_ptr(*_camPos));
}


void ShaderSkyBox::setMapTextures(const std::string& path)
{
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &_cmap);  //set the texID as a member variable
	glBindTexture(GL_TEXTURE_CUBE_MAP, _cmap);

	const char *suffixes[] = { "left", "right", "top", "down", "back", "front" };

	GLuint targets[] = {
	GL_TEXTURE_CUBE_MAP_POSITIVE_X, GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
	GL_TEXTURE_CUBE_MAP_POSITIVE_Y, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
	GL_TEXTURE_CUBE_MAP_POSITIVE_Z, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
	};

	for (int i = 0; i < 6; i++) {
		int channel;
		int width, height;
		std::string filename;
		filename = path + suffixes[i] + ".JPG";
		unsigned char *img = stbi_load(filename.c_str(), &width, &height, &channel, 0);

		if (img) {
			if (channel == 4) {
				glTexImage2D(targets[i], 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);
			}
			else {
				glTexImage2D(targets[i], 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
			}
		}
		else {
			std::cerr << "Failed to load cube map [" << filename.c_str() << ']' << std::endl;
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(img);
	}
}

/*********************/
/**    Refraction     **/
/*********************/

ShaderRefract::ShaderRefract(const std::string& path, glm::vec3 *camPos)
	: AShaderWrapper(), _camPos(camPos)
{
	// instantiating shaderProgram with uniforms
	_sh = createShaderProgram("cubemap",
		std::vector<std::string>({
			"drawSkyBox",
			// vert
			"model",
			"view",
			"projection",
			// frag
			"Alpha",
			"cubeMapTex",
			"camPos"
			})
	);

	setMapTextures(path);
}


void ShaderRefract::setUniforms(const glm::mat4& projection) {
	glUniformMatrix4fv(_sh->uniform("projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniform1i(_sh->uniform("cubeMapTex"), 0);
	glUniform3fv(_sh->uniform("camPos"), 1, glm::value_ptr(*_camPos));
}


void ShaderRefract::setMapTextures(const std::string& path)
{
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &_cmap);  //set the texID as a member variable
	glBindTexture(GL_TEXTURE_CUBE_MAP, _cmap);

	const char *suffixes[] = { "left", "right", "top", "down", "back", "front" };

	GLuint targets[] = {
	GL_TEXTURE_CUBE_MAP_POSITIVE_X, GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
	GL_TEXTURE_CUBE_MAP_POSITIVE_Y, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
	GL_TEXTURE_CUBE_MAP_POSITIVE_Z, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
	};

	for (int i = 0; i < 6; i++) {
		int channel;
		int width, height;
		std::string filename;
		filename = path + suffixes[i] + ".JPG";
		unsigned char *img = stbi_load(filename.c_str(), &width, &height, &channel, 0);

		if (img) {
			if (channel == 4) {
				glTexImage2D(targets[i], 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);
			}
			else {
				glTexImage2D(targets[i], 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
			}
		}
		else {
			std::cerr << "Failed to load cube map [" << filename.c_str() << ']' << std::endl;
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(img);
	}
}