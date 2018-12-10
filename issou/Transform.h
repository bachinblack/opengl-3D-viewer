#pragma once

#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>

struct Rotation {
	Rotation(float d, GLfloat nx, GLfloat ny, GLfloat nz)
		: degree(d), x(nx), y(ny), z(nz) {};

	GLfloat x, y, z;
	float degree;
	bool constantRotation = false;
	float currentRotation = 0;
};

class Transform {
public:
	Transform(const glm::vec3& pos, const glm::vec3& sca, const Rotation& rot)
		: position(pos), rotation(rot), scale(sca)
	{
		rotationAxis = glm::vec3(0);
	};

	glm::vec3	position;
	glm::vec3	scale;
	Rotation	rotation;
	glm::vec3	rotationAxis;
	
	void setPosition(const glm::vec3&);
	void translate(const glm::vec3&);
	glm::vec3 getPosition(void) const;

	void setScale(const glm::vec3&);
	void setScale(const float);
	glm::vec3 getScale(void) const;

	void setRotation(const Rotation&);
	void setRotation(const float deg, const float x, const float y, const float z);
	Rotation getRotation(void) const;
	glm::vec3 getRotationVec3(void) const;

	void setRotationAxis(const glm::vec3&);
	void resetRotationAxis();
	void setConstantRotation(const bool cr);
};