#pragma once

#include <glm/gtc/type_ptr.hpp>
#include "GL/glew.h"
#include "ModelView.h"
#include "Viewer.h"
#include "Loader.h"
#include "AItem.h"

class ColorCube : public AItem
{
public:
	ColorCube(const Transform& trans, const Shader &pr);
	~ColorCube() {};

private:

	void setup() override;
	void perform_draw() override;
};