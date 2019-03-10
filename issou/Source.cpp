#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "MyGlWindow.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

MyGlWindow *win = nullptr;

bool lbutton_down;
bool rbutton_down;
bool mbutton_down;
double m_lastMouseX;
double m_lastMouseY;
double cx, cy;

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}

// change m_size so that the image always fits in the screen
static void winSize(GLFWwindow* window, int width, int height)
{
	win->resize(width, height);
}

static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
	cx = xpos;
	cy = ypos;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (action == GLFW_PRESS) {
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		m_lastMouseX = xpos;
		m_lastMouseY = ypos;
	}

	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		if (GLFW_PRESS == action)
			lbutton_down = true;
		else if (GLFW_RELEASE == action)
			lbutton_down = false;
	}

	else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
		if (GLFW_PRESS == action)
			rbutton_down = true;
		else if (GLFW_RELEASE == action)
			rbutton_down = false;
	}

	else if (button == GLFW_MOUSE_BUTTON_MIDDLE) {
		if (GLFW_PRESS == action)
			mbutton_down = true;
		else if (GLFW_RELEASE == action)
			mbutton_down = false;
	}
}


void mouseDragging(double width, double height)
{
	ImGuiIO& io = ImGui::GetIO();
	
	if (io.WantCaptureMouse) {
		return ;
	}
	if (lbutton_down) {
		float fractionChangeX = static_cast<float>(cx - m_lastMouseX) / static_cast<float>(width);
		float fractionChangeY = static_cast<float>(m_lastMouseY - cy) / static_cast<float>(height);
		win->_viewer->rotate(fractionChangeX, fractionChangeY);
	}
	else if (mbutton_down) {
		float fractionChangeX = static_cast<float>(cx - m_lastMouseX) / static_cast<float>(width);
		float fractionChangeY = static_cast<float>(m_lastMouseY - cy) / static_cast<float>(height);
		win->_viewer->zoom(fractionChangeY);
	}
	else if (rbutton_down) {
		float fractionChangeX = static_cast<float>(cx - m_lastMouseX) / static_cast<float>(width);
		float fractionChangeY = static_cast<float>(m_lastMouseY - cy) / static_cast<float>(height);
		win->_viewer->translate(-fractionChangeX, -fractionChangeY, 1);
	}
	m_lastMouseX = cx;
	m_lastMouseY = cy;
}

int mainloop(GLFWwindow *window, int width, int height) {

	printf("OpenGL %s, GLSL %s\n", glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION));
	
	win = new MyGlWindow(width, height);
	bool show_test_window = false;

	float col[3] = { .2f, .2f, .2f };
	
	glm::vec3 lightPos(3, 3, 3);
	glm::vec3 lastLightPos(3, 3, 3);
	float lightColor[3] = { 1, 1, 1 };
	float lastLightColor[3] = { 1, 1, 1 };

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(col[0], col[1], col[2], 1.0); // background color
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glEnable(GL_DEPTH_TEST);

		int w, h;

		//setting light attribute
		if (lightPos != lastLightPos) {
			lastLightPos = lightPos;
			for (auto it : win->lightSources) {
				it->transform.position.y = lightPos.y;
			}
			//win->lightSource->transform.setPosition(lightPos);
		}

		glfwGetFramebufferSize(window, &w, &h);
		glViewport(0, 0, w, h);

		//imgui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		if (ImGui::Begin("Control Window"))
		{
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::Text("Light control");
			ImGui::SliderFloat("Light X", &lightPos.x, -10.0f, 10.0f);
			ImGui::SliderFloat("Light Y", &lightPos.y, -10.0f, 10.0f);
			ImGui::SliderFloat("Light Z", &lightPos.z, -10.0f, 10.0f);
			ImGui::ColorEdit3("Light color", lightColor, 0);
			//	ImGui::SliderFloat("Box Horizontal", &horizontal, 0.0f, 1.0f);
		//	ImGui::SliderFloat("Box Vertical", &vertical, 0.0f, 1.0f);
		//	
		//	ImGui::RadioButton("radio a", &radio, 0); ImGui::SameLine();
		//	ImGui::RadioButton("radio b", &radio, 1);
			ImGui::End();
		}

		/* clear screen and draw */
		win->draw();
		
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		/* Swap front and back buffers */
		glfwSwapBuffers(window);
		/* Poll for and process events */
		glfwPollEvents();
		mouseDragging(w, h);
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

void initGL() {
	if (!glfwInit())
	{
		std::cerr << "failed to init glfw. exiting..." << std::endl;
		exit(1);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void setupImgui(GLFWwindow *window)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	const char* glsl_version = "#version 430";

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();
	//ImGui::StyleColorsLight();

	glfwMakeContextCurrent(window);
}

int main() {
	initGL();
	int width = 800;
	int height = 800;
	
	
	/* Create a windowed mode window and its OpenGL context */
	GLFWwindow *window = glfwCreateWindow(width, height, "OpenGL FrameWork", NULL, NULL);
	setupImgui(window);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	/* Make the window's context current */
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		//Problem: glewInit failed, something is seriously wrong.
		std::cout << "glewInit failed, aborting." << std::endl;
		return 1;
	}

	glfwSwapInterval(1);
	glfwSetKeyCallback(window, key_callback); // call for kb
	glfwSetWindowSizeCallback (window, winSize); // call for kb
	glfwSetWindowTitle(window, "issouTV");
	
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorPosCallback(window, cursor_pos_callback);

	mainloop(window, width, height);
	
	return 0;
}