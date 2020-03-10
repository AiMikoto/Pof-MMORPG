#include "camera.h"
#include "transform.h"
#include "constants.h"
#include "variables.h"
#include <iostream>
#include "mouse.h"
#include "lib/log.h"

namespace gph = graphics;

std::vector<gph::Camera*> gph::cameras;

gph::CameraViewport::CameraViewport() {
	this->startX = 0;
	this->startY = 0;
	this->endX = 1;
	this->endY = 1;
}

gph::CameraViewport::CameraViewport(float startX, float startY, float endX, float endY) {
	this->startX = startX;
	this->startY = startY;
	this->endX = endX;
	this->endY = endY;
}

gph::Camera::Camera() {
	setup();
}

gph::Camera::~Camera() { }

gph::Camera::Camera(gph::CameraViewport viewport, bool isPerspective, bool isFixed) {
	setup();
	this->viewport = viewport;
	this->isPerspective = isPerspective;
	this->isFixed = isFixed;
}

gph::Camera::Camera(gph::Transform transform, gph::CameraViewport viewport, bool isPerspective, bool isFixed) {
	setup();
	this->transform = transform;
	this->viewport = viewport;	
	this->isPerspective = isPerspective;
	this->isFixed = isFixed;
}

void gph::Camera::setup() {
	transform = Transform(glm::dvec3(0, 0, 3),
		glm::dquat(glm::dvec3(0, 0, 0)),
		glm::dvec3(0, 0, 0));
	moveSpeed = defaultSpeed = 5.0f;
	maxSpeed = 10.0f;
	acceleration = 0;
	rotationSpeed = 1;
	fieldOfView = 45.0f;
	nearClipDistance = 0.1f;
	farClipDistance = 100.0f;
	isPerspective = true;
	isFixed = false;
	pitch = 0;
	yaw = pi;
	for (int i = 0; i < totalCameraMovements; i++) {
		moveBuffer[i] = false;
	}
	updateRotation();
}

void gph::Camera::moveCamera(int direction) {
	switch (direction) {
	case(cam::cameraMovements::forward):
		transform.position += forward() * glm::dvec3(1,0,1) * deltaTime * moveSpeed;
		break;
	case(cam::cameraMovements::backwards):
		transform.position -= forward()* glm::dvec3(1, 0, 1) * deltaTime * moveSpeed;
		break;
	case(cam::cameraMovements::right):
		transform.position += right()* glm::dvec3(1, 0, 1) * deltaTime * moveSpeed;
		break;
	case(cam::cameraMovements::left):
		transform.position -= right()* glm::dvec3(1, 0, 1) * deltaTime * moveSpeed;
		break;
	case(cam::cameraMovements::up):
		transform.position += up() * deltaTime * moveSpeed;
		break;
	case(cam::cameraMovements::down):
		transform.position -= up() * deltaTime * moveSpeed;
		break;
	}
	move = false;
}

void gph::Camera::rotateCamera(GLFWwindow* window) {
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	int width, height;
	glfwGetWindowSize(window, &width, &height);

	yaw += rotationSpeed * deltaTime * (width / 2 - xpos);
	if (yaw > 2 * pi)
		yaw -= 2 * pi;
	else if (yaw < 0)
		yaw += 2 * pi;
	pitch += rotationSpeed * deltaTime * (height / 2 - ypos);
	if (pitch > 2 * pi)
		pitch -=  2 * pi;
	else if (pitch < 0)
		pitch += 2* pi;
	updateRotation();
	rotate = false;
	cursorToMiddle(window);
}

glm::vec3 gph::Camera::lookAt() {
	return transform.position + forward();
}

glm::dvec3 gph::Camera::forward() {
	return glm::dvec3(cos(pitch) * sin(yaw), sin(pitch), cos(pitch) * cos(yaw));
}

glm::dvec3 gph::Camera::right() {
	return glm::dvec3(-cos(yaw), 0, sin(yaw));
}

glm::dvec3 gph::Camera::up() {
	return glm::dvec3(0, cos(pitch), sin(pitch));
}

void gph::Camera::updateRotation() {
	double x = std::cos(yaw) * std::cos(pitch);
	double y = std::sin(pitch);
	double z = std::sin(yaw) * std::cos(pitch);
	transform.rotation = glm::dquat(glm::dvec3(x, y, z));
}

glm::mat4 gph::Camera::projection(GLFWwindow* window) {
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	if(isPerspective)
		return glm::perspective(glm::radians(fieldOfView),
			((viewport.endX - viewport.startX) * width) / ((viewport.endY - viewport.startY) * height),
			nearClipDistance, farClipDistance);
}

glm::mat4 gph::Camera::view() {
	return glm::lookAt(glm::vec3(transform.position), lookAt(), glm::vec3(0, 1, 0));
}