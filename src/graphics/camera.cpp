#include "camera.h"
#include "transform.h"
#include "constants.h"
#include "variables.h"
#include <iostream>
#include "mouse.h"

namespace gph = graphics;

std::vector<gph::Camera*> gph::cameras;

gph::CameraViewport::CameraViewport() {}
gph::CameraViewport::~CameraViewport() {}
gph::CameraViewport::CameraViewport(int x, int y, int width, int height) {
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
}

gph::Camera::Camera() {
	setup();
}

gph::Camera::~Camera() { }

gph::Camera::Camera(gph::CameraViewport viewport) {
	setup();
	this->viewport = viewport;
}

gph::Camera::Camera(Transform* transform, gph::CameraViewport viewport) {
	setup();
	this->transform = transform;
	this->viewport = viewport;	
}

void gph::Camera::setup() {
	transform = new Transform(glm::dvec3(0, 0, 3),
		glm::dquat(glm::dvec3(0, 0, 0)),
		glm::dvec3(0, 0, 0));
	moveSpeed = defaultSpeed = 5.0f;
	maxSpeed = 10.0f;
	acceleration = 0;
	rotationSpeed = 0.005f;
	fieldOfView = 45.0f;
	nearClipDistance = 0.1f;
	farClipDistance = 45.0f;
	isFixed = false;
	isPerspective = true;
	pitch = 0;
	yaw = pi;
	for (int i = 0; i < totalCameraMovements; i++) {
		moveBuffer[i] = false;
	}
	this->transform->gameObject = this;
	updateLookAt();
}

void gph::Camera::moveCamera(int direction) {
	switch (direction) {
	case(cam::cameraMovements::forward):
		transform->position += forward() * deltaTime * moveSpeed;
		break;
	case(cam::cameraMovements::backwards):
		transform->position -= forward() * deltaTime * moveSpeed;
		break;
	case(cam::cameraMovements::right):
		transform->position += right() * deltaTime * moveSpeed;
		break;
	case(cam::cameraMovements::left):
		transform->position -= right() * deltaTime * moveSpeed;
		break;
	case(cam::cameraMovements::up):
		transform->position += up() * deltaTime * moveSpeed;
		break;
	case(cam::cameraMovements::down):
		transform->position -= up() * deltaTime * moveSpeed;
		break;
	}
	//std::cout << transform->position.x << ", " << transform->position.y << ", " << transform->position.z << std::endl;
	updateLookAt();
	move = false;
}

void gph::Camera::rotateCamera(GLFWwindow* window) {
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	int width, height;
	glfwGetWindowSize(window, &width, &height);

	yaw += rotationSpeed * deltaTime * width / 2 - xpos;
	if (yaw > 2 * pi)
		yaw -= 2 * pi;
	else if (yaw < 0)
		yaw += 2 * pi;
	pitch += rotationSpeed * deltaTime * height / 2 - ypos;
	if (pitch > 2 * pi)
		pitch -=  2 * pi;
	else if (pitch < 0)
		pitch += 2* pi;
	double x = std::cos(yaw) * std::cos(pitch);
	double y = std::sin(pitch);
	double z = std::sin(yaw) * std::cos(pitch);
	//transform->rotation = glm::dquat(glm::dvec3(x, y, z));
	rotate = false;
	updateLookAt();
	cursorToMiddle(window);
}

void gph::Camera::updateLookAt() {
	lookAt = transform->position + forward();
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