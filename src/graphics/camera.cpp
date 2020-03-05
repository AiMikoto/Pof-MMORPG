#include "camera.h"
#include "transform.h"
#include "constants.h"
#include "variables.h"

namespace gph = graphics;

std::vector<gph::Camera*> cameras;

gph::Camera::Camera() {
	//set camera position and rotation
	transform = new Transform(glm::dvec3(1, 1, 1),
		glm::dquat(glm::dvec3(0, pi, 0)),
		glm::dvec3(0, 0, 0));
	lookAt = glm::vec3(0, 0, 0);
	setup();
}

gph::Camera::~Camera() { }

gph::Camera::Camera(Transform* transform, glm::vec3 lookAt) {
	this->transform = transform;
	this->lookAt = lookAt;
	setup();
}

void gph::Camera::setup() {
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
		move[i] = false;
	}
}

void gph::Camera::moveCamera(int direction) {
	switch (direction) {
	case(cameraMovements::forward):
		transform->position += transform->forward() * deltaTime * moveSpeed;
		break;
	case(cameraMovements::backwards):
		transform->position -= transform->forward() * deltaTime * moveSpeed;
		break;
	case(cameraMovements::right):
		transform->position += transform->right() * deltaTime * moveSpeed;
		break;
	case(cameraMovements::left):
		transform->position -= transform->right() * deltaTime * moveSpeed;
		break;
	case(cameraMovements::up):
		transform->position += transform->up() * deltaTime * moveSpeed;
		break;
	case(cameraMovements::down):
		transform->position -= transform->up() * deltaTime * moveSpeed;
		break;
	}
}

void gph::Camera::rotateCamera(GLFWwindow* window) {
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	glm::dvec3 angles = glm::eulerAngles(transform->rotation);
	angles.y += rotationSpeed * width / 2 - xpos;
	if (angles.y > 2 * pi)
		angles.y -= 2 * pi;
	else if (angles.y < 0)
		angles.y += 2 * pi;
	angles.x += rotationSpeed * height / 2 - ypos;
	if (angles.x > pi / 2)
		angles.y -= pi /2;
	else if (angles.y < 0)
		angles.y += pi /2;
	transform->rotation = glm::dquat(angles);
}