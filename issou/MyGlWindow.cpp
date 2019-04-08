#include "MyGlWindow.h"

static float DEFAULT_VIEW_POINT[3] = { 0, 5, 5 };
static float DEFAULT_VIEW_CENTER[3] = { 0, 0, 0 };
static float DEFAULT_UP_VECTOR[3] = { 0, 1, 0 };


MyGlWindow::MyGlWindow(int w, int h)
	: _width(w), _height(h)
{
	// creating viewer (to get viewMatrix)

	glm::vec3 viewPoint(DEFAULT_VIEW_POINT[0], DEFAULT_VIEW_POINT[1], DEFAULT_VIEW_POINT[2]);
	glm::vec3 viewCenter(DEFAULT_VIEW_CENTER[0], DEFAULT_VIEW_CENTER[1], DEFAULT_VIEW_CENTER[2]);
	glm::vec3 upVector(DEFAULT_UP_VECTOR[0], DEFAULT_UP_VECTOR[1], DEFAULT_UP_VECTOR[2]);

	float aspect = (w / (float)h);
	_viewer = new Viewer(viewPoint, viewCenter, upVector, 45.0f, aspect);

	this->init();
}


MyGlWindow::~MyGlWindow()
{
	for (AShaderWrapper *it : _shaders) { delete it; }
	delete _viewer;
}


void MyGlWindow::getViewProjection(void) {
	// free camera (movements and rotations are player-controlled)
	if (_attached == nullptr) {
		_view = lookAt(
			_viewer->getViewPoint(), // eye
			_viewer->getViewCenter(), // look
			_viewer->getUpVector() // up
		);
	}
	// Object-_attached (movement are controlled by an object. rotations are player-controlled)
	else {
		_model.glPushMatrix();

		glm::vec3 tmpRotAx = _attached->transform.position - _attached->transform.rotationAxis;

		_model.glTranslate(-tmpRotAx.x, -tmpRotAx.y, -tmpRotAx.z);
		_model.glRotate(-_attached->transform.rotation.degree, _attached->transform.rotation.x, _attached->transform.rotation.y, _attached->transform.rotation.z);
		_model.glTranslate(tmpRotAx.x, tmpRotAx.y, tmpRotAx.z);

		glm::vec4 cameraPosition(_attached->transform.position.x, _attached->transform.position.y, _attached->transform.position.z, 0);
		glm::vec4 cameraView(cameraPosition.x + 1, cameraPosition.y, cameraPosition.z, 0);
		cameraView = cameraView * _model.getMatrix();
		cameraPosition = cameraPosition * _model.getMatrix();
		_view = lookAt(
			glm::vec3(cameraPosition.x, cameraPosition.y, cameraPosition.z),
			glm::vec3(cameraView.x, cameraView.y, cameraView.z),
			_viewer->getUpVector()
		);

		_model.glPopMatrix();
	}

	_projection = glm::perspective(45.0f, 1.0f*_width / _height, 0.1f, 500.0f);
}

void MyGlWindow::draw()
{	
	this->getViewProjection();
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// drawing every items from every shaders
	for (auto it : _shaders) {
		it->campos = _viewer->getViewPoint();
		it->computeLight(lightSources, _view);
		it->draw(_view, _projection);
	}
}

void MyGlWindow::resize(int width, int height)
{
	_width = width;
	_height = height;
}

void MyGlWindow::init(void) {
	// creating temporary elements for AItems

	Transform tmp(
		glm::vec3(-40, -6, -40),
		glm::vec3(1, 1, 1),
		Rotation(0, 1, 1, 1)
	);

	Visual vis(glm::vec3(1), 2.5);

	AShaderWrapper *sh = new ShaderSimple();
	_shaders.push_back(sh);
	Shader shader(&_view, &_projection, sh->getShaderProgram());


	// creating lights

	glm::vec3 cols[5] = {
		glm::vec3(.7, .7, .7),
		glm::vec3(.8, .2, .2),
		glm::vec3(.2, .8, .2),
		glm::vec3(.5, .2, 1),
		glm::vec3(1, 1, 1)
	};

	for (short i = 0; i < 5; i += 1)
	{
		tmp.setPosition(glm::vec3(10 * cos(i*1.25664), 8, 10 * sin(i*1.25664)));
		lightSources.push_back(new Light(tmp, shader, cols[4]));
	}

	std::cout << lightSources.size() << std::endl;
	for (auto it : lightSources) { sh->addItem(it); }
	// creating some shaders and filling them with some objects

	// create new shader
	sh = new ShaderToon();
	// add the shader to the list
	_shaders.push_back(sh);
	// use this shader to create the next items (not smart)
	shader.program = sh->getShaderProgram();


	//tmp.setPosition(glm::vec3(-3, -6, -3));
	//tmp.setScale(1.0f);
	//tmp.setRotation(45, 0, 1, 0);
	//vis.setShininess(1);
	//vis.setColor(glm::vec3(1, .4, .1));
	//sh->addItem(new Mesh(tmp, shader, "C:/Users/54604/Documents/goku.obj", vis));

	tmp.setPosition(glm::vec3(0, -6, 0));
	vis.setColor(glm::vec3(1, 0, 1));
	vis.setShininess(10);
	tmp.setScale(.6f);
	tmp.setRotation(Rotation(-90, 1, 0, 0));
	sh->addItem(new VBOTeapot(tmp, shader, 16, glm::mat4(1), vis));


	//AShaderWrapper *silhouette = new ShaderSilouette();
	//_shaders.push_back(silhouette);

	//silhouette->addItems(sh->getItems());

	//sh = new ShaderPhong();
	//_shaders.push_back(sh);
	//shader.program = sh->getShaderProgram();

	//tmp.setPosition(glm::vec3(5, 0, 0));
	//sh->addItem(new LightItem(tmp, shader));

	//tmp.setPosition(glm::vec3(-5, 0, 0));
	//vis.setColor(glm::vec3(1, 0, 0));
	//sh->addItem(new LightItem(tmp, shader, vis));

	//tmp.setPosition(glm::vec3(0, 5, 0));
	//vis.setColor(glm::vec3(0, 1, 0));
	//sh->addItem(new LightItem(tmp, shader, vis));

	//sh = new ShaderFog();
	//_shaders.push_back(sh);
	//shader.program = sh->getShaderProgram();
	//tmp.setPosition(glm::vec3(0, 0, 0));
	//tmp.setScale(12);
	//sh->addItem(new LightItem(tmp, shader, vis));

	//tmp.setPosition(glm::vec3(0, 12, 0));
	//vis.setColor(glm::vec3(1, 0, 1));
	//vis.setShininess(10);
	//tmp.setScale(.6f);
	//tmp.setRotation(Rotation(-90, 1, 0, 0));
	//sh->addItem(new VBOTeapot(tmp, shader, 16, glm::mat4(1), vis));

	//tmp.setPosition(glm::vec3(5, 12, 0));
	//vis.setColor(glm::vec3(1, .7, 0));
	//sh->addItem(new VBOTeapot(tmp, shader, 16, glm::mat4(1), vis));

	//sh = new ShaderHandDrawn();
	//_shaders.push_back(sh);
	//shader.program = sh->getShaderProgram();
	//tmp.setPosition(glm::vec3(0, 0, 0));
	//

	//tmp.setScale(1);
	//tmp.setPosition(glm::vec3(0, 0, 0));
	//sh->addItem(new LightItem(tmp, shader, vis));


	tmp.setPosition(glm::vec3(4, 4, 0));
	tmp.setScale(.01f);
	tmp.setRotation(45, 0, 1, 0);
	vis.setShininess(1);
	vis.setColor(glm::vec3(.5, .5, .5));
	//sh->addItem(new Mesh(tmp, shader, "./resources/goku.obj", vis));

	//sh = new ShaderTextured();
	//_shaders.push_back(sh);
	//shader.program = sh->getShaderProgram();

	//sh->addItem(new TexturedItem(tmp, shader, "./resources/goku.obj", "./resources/brick1.jpg", vis));
	//sh->addItem(new TexturedSphere(tmp, shader, "./resources/earth.jpg", vis));
	//sh->addItem(new TexturedMesh(tmp, shader, "./resources/SkullKid/skull_kid.obj", "./resources/SkullKid/model.nut0.png", vis));
	//sh->addItem(new TexturedMesh(tmp, shader, "C:/Graphics/res/Sponza-master/sponza.obj", vis));
	//sh->addItem(new TexturedMesh(tmp, shader, "C:/Graphics/res/lucasMother/bs_ears.obj", "C:/Graphics/res/lucasMother/ogre_diffuse.png", vis));


	sh = new ShaderSkyBox("C:/Graphics/res/textures/", _viewer->getViewPointPtr());
	_shaders.push_back(sh);
	shader.program = sh->getShaderProgram();

	tmp.setScale(2);
	sh->addItem(new CubeMappedMesh(tmp, shader, "C:/Graphics/res/teapot.obj", vis));
	sh->addItem(new Skybox(tmp, shader, vis));
}