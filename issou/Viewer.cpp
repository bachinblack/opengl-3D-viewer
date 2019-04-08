// File: Viewer.cpp
// Author: mankyu sung

#include <iostream>
#include "Viewer.h"
#include <glm/gtc/quaternion.hpp>

static float DEFAULT_TRANSLATE_SPEED = 0.5;
static float DEFAULT_ZOOM_FRACTION = 2.5;
static float DEFAULT_ROTATE_SPEED = 2.0;

using namespace std;

Viewer::Viewer(
	const glm::vec3 &viewPoint, const glm::vec3 &viewCenter, const glm::vec3 &upVector,
	float fieldOfView, float aspectRatio
) :
	m_viewPoint(viewPoint),
	m_viewCenter(viewCenter),
	m_upVector(upVector),
	m_fieldOfView(fieldOfView),
	m_aspectRatio(aspectRatio),
	m_translateSpeed(DEFAULT_TRANSLATE_SPEED),
	m_zoomFraction(DEFAULT_ZOOM_FRACTION),
	m_rotateSpeed(DEFAULT_ROTATE_SPEED)
{
	m_upVector = glm::normalize(m_upVector);

	getFrustrumInfo();
}

void Viewer::translate(float changeHoriz, float changeVert, bool inImagePlane) {
	glm::vec3 translateVec;

	if (inImagePlane) {

		translateVec = (m_imagePlaneHorizDir * (m_displayWidth * changeHoriz)) + (m_imagePlaneVertDir * (changeVert * m_displayHeight));
	}
	else {
		translateVec = (m_viewCenter - m_viewPoint) * changeVert;
	}
	translateVec *= m_translateSpeed;

	m_viewPoint += translateVec;
	m_viewCenter += translateVec;
}

void Viewer::zoom(float changeVert) {

	float scaleFactor = powf(2.0, -changeVert * m_zoomFraction);
	m_viewPoint = m_viewCenter + (m_viewPoint - m_viewCenter) * scaleFactor;

	getFrustrumInfo();
}

const float pi = glm::pi<float>();

glm::quat setFromAxisAngle(glm::vec3 & axis, float angle) {

	float cosAng = cosf(angle / 2.0f);
	float sinAng = sinf(angle / 2.0f);
	float norm = sqrt(axis.x * axis.x + axis.y * axis.y + axis.z * axis.z);

	glm::quat res;

	res.x = axis.x / norm;
	res.y = axis.y / norm;
	res.z = axis.z / norm;

	res.w = cosAng; //w

	res.x *= sinAng; //x
	res.y *= sinAng; //y
	res.z *= sinAng; //z

	return res;
}


void makeOrthogonalTo(glm::vec3 & vec1, glm::vec3 & vec2) {
	float length = glm::length(vec2);

	if (length == 0) {
		std::cout << "Warning: called MathVec3D::makeOrthogonalTo with zero vector as the argument.  Doing nothing." << std::endl;
	}
	else {
		float scale = (vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z) / (length * length);
		vec1.x -= scale * vec2.x;
		vec1.y -= scale * vec2.y;
		vec1.z -= scale * vec2.z;
	}
}



void Viewer::rotate(float changeHoriz, float changeVert) {
	float horizRotAngle = m_rotateSpeed * changeVert;
	float vertRotAngle = -m_rotateSpeed * changeHoriz;

	glm::quat horizRot;
	horizRot = setFromAxisAngle(m_imagePlaneHorizDir, horizRotAngle);


	glm::quat vertRot;
	vertRot = setFromAxisAngle(m_imagePlaneVertDir, vertRotAngle);

	glm::quat totalRot = horizRot * vertRot;

	glm::vec3 viewVec = m_viewPoint - m_viewCenter;
	viewVec = totalRot * viewVec;

	m_viewPoint = m_viewCenter + viewVec;

	getFrustrumInfo();
}

void Viewer::centerAt(const  glm::vec3 &pos) {
	m_viewPoint += (pos - m_viewCenter);
	m_viewCenter = pos;
	getFrustrumInfo();
}

void Viewer::lookFrom(const glm::vec3 &pos) {
	m_viewPoint = pos;
	getFrustrumInfo();
}

glm::vec3 Viewer::getViewPoint() const {
	return(m_viewPoint);
}

glm::vec3 *Viewer::getViewPointPtr() {
	return(&m_viewPoint);
}

glm::vec3 Viewer::getViewCenter() const {
	return(m_viewCenter);
}

glm::vec3 Viewer::getUpVector() const {
	return(m_upVector);
}

float Viewer::getFieldOfView() const {
	return(m_fieldOfView);
}

float Viewer::getAspectRatio() const {
	return(m_aspectRatio);
}

glm::vec3 Viewer::getViewDir() const {
	return(m_viewDir);
}

glm::vec3 Viewer::getImagePlaneHorizDir() const {
	return(m_imagePlaneHorizDir);
}

glm::vec3 Viewer::getImagePlaneVertDir() const {
	return(m_imagePlaneVertDir);
}

void Viewer::setAspectRatio(float aspectRatio) {
	if (m_aspectRatio != aspectRatio) {
		m_aspectRatio = aspectRatio;
		getFrustrumInfo();
	}
}

void Viewer::setFieldOfView(float fieldOfView) {
	if (m_fieldOfView != fieldOfView) {
		m_fieldOfView = fieldOfView;
		getFrustrumInfo();
	}
}

void Viewer::setTranslateSpeed(float translateSpeed) {
	m_translateSpeed = translateSpeed;
}

void Viewer::setZoomFraction(float zoomFraction) {
	m_zoomFraction = zoomFraction;
}

void Viewer::setRotateSpeed(float rotateSpeed) {
	m_rotateSpeed = rotateSpeed;
}

void Viewer::getFrustrumInfo() {
	// Get the viewing direction

	m_viewDir = m_viewCenter - m_viewPoint;
	m_viewDir = glm::normalize(m_viewDir);

	// Get the vertical image-plane direction (the projection of the up vector into the view plane)
	m_imagePlaneVertDir = m_upVector;
	makeOrthogonalTo(m_imagePlaneVertDir, m_viewDir);
	m_imagePlaneVertDir = glm::normalize(m_imagePlaneVertDir);


	// Get the horizontal image-plane direction
	m_imagePlaneHorizDir = glm::cross(m_viewDir, m_imagePlaneVertDir);
	m_imagePlaneHorizDir = glm::normalize(m_imagePlaneHorizDir);

	// Get the view plane width and height at the view center.
	m_displayHeight = 2.0f * glm::length(m_viewCenter - m_viewPoint) * tan(0.5f*m_fieldOfView);
	m_displayWidth = m_displayHeight * m_aspectRatio;
}