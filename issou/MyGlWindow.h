#pragma once

#include <iostream>
#include "GL/glew.h"
#include <string>
#include "Loader.h"
#include "ColorCube.h"
#include "CheckeredFloor.h"

#include "ModelView.h"
#include <glm/gtc/type_ptr.hpp>
#include "Viewer.h"
#include "Obj.h"
#include "AItem.h"
#include "wireCube.h"
#include "LineSegment.h"
#include "LightItem.h"
#include "Light.h"
#include "Sphere.h"
#include "vboteapot.h"
#include "Mesh.h"

class MyGlWindow {
public:
	MyGlWindow(int w, int h);
	~MyGlWindow();
	void draw();
	void resize(int width, int height);
	Viewer		*m_viewer;
	
	Light		*lightSource;

private:
	Model			m_model;

	glm::vec4	lightLocation;
	glm::vec3	Ld;

	ShaderProgram	*s_simple;
	std::vector<AItem *>	simple;
	ShaderProgram	*s_light;
	std::vector<AItem *>	light;
	ShaderProgram	*s_textured;
	std::vector<AItem *>	textured;

	glm::mat4				view, projection;
	AItem					*attached;
	int	m_width;
	int m_height;
	int currentView;

	void setupBuffer();
};