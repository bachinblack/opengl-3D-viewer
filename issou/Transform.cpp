#include "Transform.h"


void Transform::setPosition(const glm::vec3& tmp) { position = tmp; }

void Transform::translate(const glm::vec3 & tmp) { position = position + tmp; }

glm::vec3 Transform::getPosition(void) const { return position; }


void Transform::setScale(const glm::vec3& tmp) { scale = tmp; }

void Transform::setScale(const float tmp) { scale = glm::vec3(tmp); }

glm::vec3 Transform::getScale() const { return scale; }


void Transform::setRotation(const Rotation& tmp) { rotation = tmp; }

void Transform::setRotation(const float deg, const float x, const float y, const float z)
{ rotation.degree = deg; rotation.x = x; rotation.y = y; rotation.z = z; }

Rotation Transform::getRotation(void) const { return rotation; }

glm::vec3 Transform::getRotationVec3(void) const { return glm::vec3(rotation.x, rotation.y, rotation.z); }


void Transform::setRotationAxis(const glm::vec3& tmp) { rotationAxis = tmp; }

void Transform::resetRotationAxis() { rotationAxis = glm::vec3(0, 0, 0); }

void Transform::setConstantRotation(const bool cr) { rotation.constantRotation = cr; }
