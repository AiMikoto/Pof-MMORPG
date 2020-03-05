#include "camera.h"
#include "transform.h"
#include "constants.h"
#include "variables.h"
#include <iostream>

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
	for (int i = 0; i < totalCameraMovements; i++) {
		moveBuffer[i] = false;
	}
	this->transform->gameObject = this;
	updateLookAt();
}

void gph::Camera::moveCamera(int direction) {
	switch (direction) {
	case(cam::cameraMovements::forward):
		transform->position += transform->forward() * deltaTime * moveSpeed;
		break;
	case(cam::cameraMovements::backwards):
		transform->position -= transform->forward() * deltaTime * moveSpeed;
		break;
	case(cam::cameraMovements::right):
		transform->position += transform->right() * deltaTime * moveSpeed;
		break;
	case(cam::cameraMovements::left):
		transform->position -= transform->right() * deltaTime * moveSpeed;
		break;
	case(cam::cameraMovements::up):
		transform->position += transform->up() * deltaTime * moveSpeed;
		break;
	case(cam::cameraMovements::down):
		transform->position -= transform->up() * deltaTime * moveSpeed;
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
	glfwGetFramebufferSize(window, &width, &height);

	glm::dvec3 angles = glm::eulerAngles(transform->rotation);
	std::cout << angles.x << ", " << angles.y << ", " << angles.z << std::endl;
	angles.y += rotationSpeed * width / 2 - xpos;
	if (angles.y > 2 * pi)
		angles.y -= 2 * pi;
	else if (angles.y < 0)
		angles.y += 2 * pi;
	angles.z += rotationSpeed * height / 2 - ypos;
	if (angles.z > pi / 2)
		angles.z -= pi /2;
	else if (angles.z < 0)
		angles.z += pi /2;
	transform->rotation = glm::dquat(angles);
	rotate = false;
}

void gph::Camera::updateLookAt() {
	lookAt = transform->position + transform->forward();
}