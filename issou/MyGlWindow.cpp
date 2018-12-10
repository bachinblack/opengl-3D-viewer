#include "MyGlWindow.h"

static float DEFAULT_VIEW_POINT[3] = { 0, 5, 5 };
static float DEFAULT_VIEW_CENTER[3] = { 0, 0, 0 };
static float DEFAULT_UP_VECTOR[3] = { 0, 1, 0 };


// VAO (vertex array object = all)
// VBO (vertex buffer object = one information)
GLuint vao, vboPosition, vboColor, vbo, ibo;

void MyGlWindow::setupBuffer()
{
	std::string SHADERS_DIR = "./shaders/";

	// simple
	s_simple = new ShaderProgram();
	s_simple->initFromFiles(SHADERS_DIR + "simple.vert", SHADERS_DIR + "simple.frag");
	s_simple->addUniform("MVP");

	// light
	s_light = new ShaderProgram();
	s_light->initFromFiles(SHADERS_DIR + "phong.vert", SHADERS_DIR + "phong.frag");
	s_light->addUniform("modelView");
	s_light->addUniform("projection");
	s_light->addUniform("lightLoc");
	s_light->addUniform("Kd");
	s_light->addUniform("Ld");
	s_light->addUniform("Ka");
	s_light->addUniform("La");
	s_light->addUniform("Ks");
	s_light->addUniform("Ls");
	s_light->addUniform("Shininess");
	s_light->addUniform("Alpha");
	s_light->addUniform("normalMatrix");

	// textured
	s_textured = new ShaderProgram();
	s_textured->initFromFiles(SHADERS_DIR + "textured.vert", SHADERS_DIR + "textured.frag");
	s_textured->addUniform("MVP");
}

void MyGlWindow::resize(int width, int height)
{
	m_width = width;
	m_height = height;
}

void MyGlWindow::draw()
{	
	
	if (attached == nullptr) {
		view = lookAt(
			m_viewer->getViewPoint(), // eye
			m_viewer->getViewCenter(), // look
			m_viewer->getUpVector() // up
		);
	} else {
		m_model.glPushMatrix();
		
		glm::vec3 tmpRotAx = attached->transform.position - attached->transform.rotationAxis;

		m_model.glTranslate(-tmpRotAx.x, -tmpRotAx.y, -tmpRotAx.z);
		m_model.glRotate(-attached->transform.rotation.degree, attached->transform.rotation.x, attached->transform.rotation.y, attached->transform.rotation.z);
		m_model.glTranslate(tmpRotAx.x, tmpRotAx.y, tmpRotAx.z);

		glm::vec4 cameraPosition(attached->transform.position.x, attached->transform.position.y, attached->transform.position.z, 0);
		glm::vec4 cameraView(cameraPosition.x + 1, cameraPosition.y, cameraPosition.z, 0);
		cameraView = cameraView * m_model.getMatrix();
		cameraPosition = cameraPosition * m_model.getMatrix();
		view = lookAt(
			glm::vec3(cameraPosition.x, cameraPosition.y, cameraPosition.z),
			glm::vec3(cameraView.x, cameraView.y, cameraView.z),
			m_viewer->getUpVector()
		);

		m_model.glPopMatrix();
	}

	projection = glm::perspective(45.0f, 1.0f*m_width / m_height, 0.1f, 500.0f);

	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	s_simple->use();
	for (AItem *it : simple) { it->draw(view); }
	s_simple->disable();
	
	s_light->use();

	glUniformMatrix4fv(s_light->uniform("projection"), 1, GL_FALSE, glm::value_ptr(projection));


	glUniform4fv(s_light->uniform("lightLoc"), 1, glm::value_ptr(view * glm::vec4(lightSource->transform.getPosition(), 1)));
	glUniform3fv(s_light->uniform("Ld"), 1, glm::value_ptr(Ld));
	glUniform3fv(s_light->uniform("Ka"), 1, glm::value_ptr(glm::vec3(.2, .2, .2)));
	glUniform3fv(s_light->uniform("La"), 1, glm::value_ptr(glm::vec3(1, 1, 1)));
	glUniform3fv(s_light->uniform("Ks"), 1, glm::value_ptr(glm::vec3(.3, .3, .3)));
	glUniform3fv(s_light->uniform("Ls"), 1, glm::value_ptr(glm::vec3(1, 1, 1)));
	
	for (AItem *it : light) { it->draw(view); }
	
	s_light->disable();
	
	s_textured->use();
	for (AItem *it : textured) { it->draw(view); }
	s_textured->disable();
}

//void MyGlWindow::setView(const int view)
//{
//	if (view == currentView) { return; }
//	currentView = view;
//	if (view == 0) {
//		attached = nullptr;
//		cube->setVisible(true);
//	} else {
//		attached = cube;
//		cube->setVisible(false);
//	}
//}

MyGlWindow::MyGlWindow(int w, int h)
{
	lightLocation = glm::vec4(3, 3, 3, 1);
	Ld = glm::vec3(1, 1, 1);
	currentView = 0;
	m_width = w;
	m_height = h;

	glm::vec3 viewPoint(DEFAULT_VIEW_POINT[0], DEFAULT_VIEW_POINT[1], DEFAULT_VIEW_POINT[2]);
	glm::vec3 viewCenter(DEFAULT_VIEW_CENTER[0], DEFAULT_VIEW_CENTER[1], DEFAULT_VIEW_CENTER[2]);
	glm::vec3 upVector(DEFAULT_UP_VECTOR[0], DEFAULT_UP_VECTOR[1], DEFAULT_UP_VECTOR[2]);

	float aspect = (w / (float)h);
	m_viewer = new Viewer(viewPoint, viewCenter, upVector, 45.0f, aspect);
	
	setupBuffer();

	Transform tmp(
		glm::vec3(-40, -6, -40),
		glm::vec3(1, 1, 1),
		Rotation(0, 1, 1, 1)
	);

	Visual vis(glm::vec3(1), 2.5);

	Shader shader(&view, &projection, s_simple);


	simple.push_back(new CheckeredFloor(tmp, shader, 80, 40));

	tmp.setPosition(glm::vec3(3, 3, 3));
	lightSource = new Light(tmp, shader);
	simple.push_back(lightSource);

	simple.push_back(new LineSegment(tmp, shader, glm::vec4(), glm::vec4()));

	shader.program = s_textured;

	//textured.push_back(new Mesh(tmp, shader, "C:/Users/54604/Documents/rupee.txt"));

	shader.program = s_light;

	
	tmp.setPosition(glm::vec3(0, 0, 0));
	//light.push_back(new Mesh(tmp, shader, "C:/Users/54604/Documents/cube.obj"));
	
	tmp.setPosition(glm::vec3(3, -6, -3));
	tmp.setScale(.3f);
	tmp.setRotation(-45, 0, 1, 0);
	vis.setShininess(2);
	vis.setColor(glm::vec3(.6, .8, .2));
	light.push_back(new Mesh(tmp, shader, "C:/Users/54604/Downloads/SkullKid/skull_kid.obj", vis));
	tmp.setScale(1);

	tmp.setPosition(glm::vec3(-3, -6, -3));
	tmp.setScale(1.0f);
	tmp.setRotation(45, 0, 1, 0);
	vis.setShininess(1);
	vis.setColor(glm::vec3(.2, .2, .9));
	light.push_back(new Mesh(tmp, shader, "C:/Users/54604/Documents/goku.obj", vis));
	tmp.setScale(1);

	//tmp.setPosition(glm::vec3(5, 0, 0));
	//vis.setShiness(1);
	//light.push_back(new LightItem(tmp, shader));
	//
	//tmp.setPosition(glm::vec3(-5, 0, 0));
	//vis.setColor(glm::vec3(1, 0, 0));
	//vis.setShiness(3);
	//light.push_back(new LightItem(tmp, shader, vis));
	//
	//tmp.setPosition(glm::vec3(0, 5, 0));
	//vis.setColor(glm::vec3(0, 1, 0));
	//vis.setShiness(5);
	//light.push_back(new LightItem(tmp, shader, vis));
	//
	//tmp.setPosition(glm::vec3(0, -5, 0));
	//vis.setColor(glm::vec3(0, 0, 1));
	//vis.setShiness(7);
	//light.push_back(new LightItem(tmp, shader, vis));
	
	//tmp.setPosition(glm::vec3(3, 3, 3));
	//vis.setColor(glm::vec3(0, 1, 1));
	//vis.setShininess(10);
	//light.push_back(new Sphere(tmp, shader, 1, 64, 64, vis));

	tmp.setPosition(glm::vec3(0, -6, 0));
	vis.setColor(glm::vec3(1, 0, 1));
	vis.setShininess(10);
	tmp.setScale(.6);
	tmp.setRotation(Rotation(-90, 1, 0, 0));
	light.push_back(new VBOTeapot(tmp, shader, 16, glm::mat4(1), vis));
	tmp.setScale(1);
}

MyGlWindow::~MyGlWindow()
{
	for (AItem *it : simple) { delete it; }
	for (AItem *it : light) { delete it; }
	for (AItem *it : textured) { delete it; }
	delete s_simple, s_light, s_textured;
	delete m_viewer;
}
