#include "MyGlWindow.h"

static float DEFAULT_VIEW_POINT[3] = { 0, 5, 5 };
static float DEFAULT_VIEW_CENTER[3] = { 0, 0, 0 };
static float DEFAULT_UP_VECTOR[3] = { 0, 1, 0 };

// VAO (vertex array object = all)
// VBO (vertex buffer object = one information)
GLuint vao, vboPosition, vboColor, vbo, ibo;

std::string SHADERS_DIR = "./shaders/";

ShaderProgram *createShaderProgram(const std::string shname, const std::vector<std::string>& uniforms)
{
	ShaderProgram *shader = new ShaderProgram();
	
	shader->initFromFiles(SHADERS_DIR + shname + ".vert", SHADERS_DIR + shname + ".frag");
	for (std::string elem : uniforms) {
		shader->addUniform(elem);
	}
	
	return shader;
}

void MyGlWindow::setupBuffer()
{
	// simple
	s_simple = createShaderProgram("simple", std::vector<std::string>({ "MVP" }));

	// light
	s_light = createShaderProgram("multilight",
		std::vector<std::string>({
			"modelView",
			"projection",
			"Kd",
			"Ka",
			"La",
			"Ks",
			"Ls",
			"Shininess",
			"Alpha",
			"normalMatrix",
			"lightPos",
			"lightCol",
		})
	);

	// spotlight
	//s_spot = createShaderProgram("spot",
	//	std::vector<std::string>({
	//		"pos",
	//		"intensity",
	//		"direction",
	//		"exponent",
	//		"cutoff",
	//		"Innercutoff",
	//	})
	//);

	// textured
	s_textured = createShaderProgram("textured", std::vector<std::string>({ "MVP" })
	);
}

void MyGlWindow::resize(int width, int height)
{
	m_width = width;
	m_height = height;
}

void MyGlWindow::draw()
{	
	// free camera (movements and rotations are player-controlled)
	if (attached == nullptr) {
		view = lookAt(
			m_viewer->getViewPoint(), // eye
			m_viewer->getViewCenter(), // look
			m_viewer->getUpVector() // up
		);
	// Object-attached (movement are controlled by an object. rotations are player-controlled)
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


	//GLfloat *tmp = new  GLfloat[lightSources.size() * 4 * 2];
	glm::vec4 loc;
	glm::vec3 col;
	int p = 0, c = 0;

	glm::vec4 *lightPos = new glm::vec4[5];
	glm::vec3 *lightCol = new glm::vec3[5];
	s_simple->use();
	for (AItem *it : simple) { it->draw(view); }
	for (Light *it : lightSources) {
		it->draw(view);
		
		//loc = (view * glm::vec4(it->transform.getPosition(), 1));
		loc = glm::vec4(it->transform.getPosition(), 1);
		col = it->getColor();
		lightPos[p++] = loc;
		lightCol[c++] = col;
	}
	s_simple->disable();
	
	s_light->use();

	float colors[6] = {
		1, 0, 0,
		0, 1, 0
	};

	glUniformMatrix4fv(s_light->uniform("projection"), 1, GL_FALSE, glm::value_ptr(projection));

	for (Light *it : lightSources) {
		glUniform4fv(s_light->uniform("lightPos"), 5, glm::value_ptr(*lightPos));
		glUniform3fv(s_light->uniform("lightCol"), 5, glm::value_ptr(*lightCol));
	}

	//glUniform4fv(s_light->uniform("lightLoc"), 1, glm::value_ptr(view * glm::vec4(lightSource->transform.getPosition(), 1)));
	//glUniform3fv(s_light->uniform("Ld"), 1, glm::value_ptr(Ld));
	glUniform3fv(s_light->uniform("Ka"), 1, glm::value_ptr(glm::vec3(.2, .2, .2)));
	glUniform3fv(s_light->uniform("La"), 1, glm::value_ptr(glm::vec3(1, 1, 1)));
	glUniform3fv(s_light->uniform("Ks"), 1, glm::value_ptr(glm::vec3(.5, .5, .5)));
	glUniform3fv(s_light->uniform("Ls"), 1, glm::value_ptr(glm::vec3(1, 1, 1)));
	

	for (AItem *it : light) { it->draw(view); }
	
	s_light->disable();
	
	//s_spot->use();
	//
	//glUniform4fv(s_spot->uniform("pos"), 1, glm::value_ptr(glm::vec4(spotLight->transform.getPosition(), 1)));
	//glUniform3fv(s_spot->uniform("intensity"), 1, glm::value_ptr(spotLight->col));
	//glUniform3fv(s_spot->uniform("direction"), 1, glm::value_ptr(spotLight->direction));
	//glUniform1f(s_spot->uniform("exponent"), 1);
	//glUniform1f(s_spot->uniform("cutoff"), 2);
	//glUniform1f(s_spot->uniform("Innercutoff"), 1);
	//for (AItem * it : spot) { it->draw(view); }
	//s_spot->disable();

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



	tmp.setPosition(glm::vec3(3, 3, 3));
	//lightSource = new Light(tmp, shader, glm::vec3(.8, .7, .1));

	glm::vec3 cols[5] = {
		glm::vec3(.7, .7, .7),
		glm::vec3(.8, .2, .2),
		glm::vec3(.2, .8, .2),
		glm::vec3(.2, .2, .8),
		glm::vec3(.8, .2, .8)
	};
	
	for (short i = 0; i < 5; ++i)
	{
		tmp.setPosition(glm::vec3(10 * cos(i*1.25664), 8, 10 * sin(i*1.25664)));
		lightSources.push_back(new Light(tmp, shader, cols[i]));
	}

	//simple.push_back(lightSource);

	//shader.program = s_spot;
	
	//spot.push_back(new CheckeredFloor(tmp, shader, 80, 40));


	//tmp.setPosition(glm::vec3(3, 10, 3));
	//spotLight = new SpotLight(tmp, shader, glm::vec3(.8, .7, .2));

	//simple.push_back(new LineSegment(tmp, shader, glm::vec4(), glm::vec4()));

	shader.program = s_textured;

	//textured.push_back(new Mesh(tmp, shader, "C:/Users/54604/Documents/rupee.txt"));

	shader.program = s_light;

	
	tmp.setPosition(glm::vec3(0, 0, 0));
	//light.push_back(new Mesh(tmp, shader, "C:/Users/54604/Documents/cube.obj"));
	
	//tmp.setPosition(glm::vec3(3, -6, -3));
	//tmp.setScale(.3f);
	//tmp.setRotation(-45, 0, 1, 0);
	//vis.setShininess(2);
	//vis.setColor(glm::vec3(.6, .8, .2));
	//light.push_back(new Mesh(tmp, shader, "C:/Users/54604/Downloads/SkullKid/skull_kid.obj", vis));
	tmp.setScale(1);

	tmp.setPosition(glm::vec3(-3, -6, -3));
	tmp.setScale(1.0f);
	tmp.setRotation(45, 0, 1, 0);
	vis.setShininess(1);
	vis.setColor(glm::vec3(.2, .2, .9));
	light.push_back(new Mesh(tmp, shader, "C:/Users/54604/Documents/goku.obj", vis));
	tmp.setScale(1);

	//tmp.setPosition(glm::vec3(5, 0, 0));
	//light.push_back(new LightItem(tmp, shader));
	//
	//tmp.setPosition(glm::vec3(-5, 0, 0));
	//vis.setColor(glm::vec3(1, 0, 0));
	//light.push_back(new LightItem(tmp, shader, vis));
	//
	//tmp.setPosition(glm::vec3(0, 5, 0));
	//vis.setColor(glm::vec3(0, 1, 0));
	//light.push_back(new LightItem(tmp, shader, vis));
	//
	//tmp.setPosition(glm::vec3(0, -5, 0));
	//vis.setColor(glm::vec3(0, 0, 1));
	//light.push_back(new LightItem(tmp, shader, vis));
	
	//tmp.setPosition(glm::vec3(3, 3, 3));
	//vis.setColor(glm::vec3(0, 1, 1));
	//vis.setShininess(10);
	//light.push_back(new Sphere(tmp, shader, 1, 64, 64, vis));

	tmp.setPosition(glm::vec3(0, -6, 0));
	vis.setColor(glm::vec3(1, 0, 1));
	vis.setShininess(10);
	tmp.setScale(.6f);
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
