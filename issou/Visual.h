#pragma once

#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>

class Visual
{
public:
	Visual(const glm::vec3& col = glm::vec3(1), const float sh = 5.0, const float al = 1) : color(col), shininess(sh), alpha(al) { };
	~Visual();

	glm::vec3 getColor(void) const;
	void setColor(const glm::vec3& value);

	float getShininess(void) const;
	void setShininess(const float value);

	float getAlpha(void) const;
	void setAlpha(const float value);

private:
	glm::vec3 color;
	float shininess, alpha;
};

