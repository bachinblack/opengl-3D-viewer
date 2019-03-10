                                       #pragma once
#include <vector>

#include "AItem.h"

//class LightManager
//{
//public:
//	LightManager(std::vector<Light *>& lights);
//	~LightManager();
//
//	GLfloat *getLocationsArray(const glm::vec4& view) const;
//
//private:
//	std::vector<Light *> lightSources;
//	GLfloat *lightLocations;
//
//};

class Light : public AItem
{
public:
	Light(const Transform& trans, const Shader &pr, const glm::vec3& col);
	~Light() {};

	glm::vec3 getColor() const { return color; };
	void setColor(const glm::vec3& col) { color = col; }

private:

	glm::vec3 color;

	void setup() override;
	void perform_draw() override;
};                                                                                                                                                                                                                                                                