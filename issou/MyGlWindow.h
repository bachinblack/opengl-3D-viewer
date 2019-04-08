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
#include "SpotLight.h"
#include "TexturedItem.h"
#include "TexturedSphere.h"
#include "TexturedMesh.h"
#include "CubeMappedMesh.h"
#include "ShaderWrappers.h"
#include "Skybox.h"

class MyGlWindow {
public:
	MyGlWindow(int w, int h);
	~MyGlWindow();
	
	void draw();
	void resize(int width, int height);
	
	Viewer		*_viewer;
	std::vector<Light *> lightSources;

private:
	int	_width;
	int _height;

	glm::mat4		_view, _projection;
	Model			_model;
	AItem		   *_attached;

	std::vector<AShaderWrapper *> _shaders;

	void getViewProjection(void);
	void init(void);
};