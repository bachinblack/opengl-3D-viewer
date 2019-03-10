#include "ShaderWrappers.h"


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


void AShaderWrapper::computeLight(std::vector<Light *> lights) {
	short i = 0;
	
	_lightNb = static_cast<unsigned short>(lights.size());

	if (_lightPos != nullptr) { delete _lightPos; }
	_lightPos = new glm::vec4[_lightNb];
	
	if (_lightCol != nullptr) { delete _lightCol; }
	_lightCol = new glm::vec3[_lightNb];

	for (Light *it : lights) {
		_lightPos[i] = glm::vec4(it->transform.getPosition(), 1);
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
