#include "camera.h"
#include "transform.h"
#include "constants.h"
#include "variables.h"
#include <iostream>
#include "mouse.h"
#include "lib/log.h"
#include "gpu.h"
#include <typeinfo>
#include "utils.h"

namespace gph = graphics;

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

gph::Camera::Camera() : Component(true) {
	setType();
}

gph::Camera::~Camera() { }

gph::Camera::Camera(gph::CameraViewport viewport, bool isPerspective, bool isFixed) : Component(true) {
	this->viewport = viewport;
	this->isPerspective = isPerspective;
	this->isFixed = isFixed;
	setType();
}

gph::Camera::Camera(gph::Transform transform, gph::CameraViewport viewport, bool isPerspective, bool isFixed) : Component(true) {
	this->gameObject->transform = transform;
	this->viewport = viewport;	
	this->isPerspective = isPerspective;
	this->isFixed = isFixed;
	setType();
}

void gph::Camera::setup() {
	moveSpeed = 5.0f;
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
	gpu->cameras.push_back(this);
	Component::setup();
}

void gph::Camera::moveCamera(int direction) {
	glm::dvec3 cancelDirectionMovements = { 1,1,1 };
	switch (direction) {
	case(cam::cameraMovements::forward):
		gameObject->transform.position += forward() * cancelDirectionMovements * deltaTime * moveSpeed;
		break;
	case(cam::cameraMovements::backwards):
		gameObject->transform.position -= forward() * cancelDirectionMovements * deltaTime * moveSpeed;
		break;
	case(cam::cameraMovements::right):
		gameObject->transform.position += right() * cancelDirectionMovements * deltaTime * moveSpeed;
		break;
	case(cam::cameraMovements::left):
		gameObject->transform.position -= right() * cancelDirectionMovements * deltaTime * moveSpeed;
		break;
	case(cam::cameraMovements::up):
		gameObject->transform.position += up() * deltaTime * moveSpeed;
		break;
	case(cam::cameraMovements::down):
		gameObject->transform.position -= up() * deltaTime * moveSpeed;
		break;
	}
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
	return gameObject->transform.position + forward();
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
	gameObject->transform.rotation = glm::dquat(glm::dvec3(x, y, z));
}

glm::mat4 gph::Camera::projection(GLFWwindow* window) {
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	if(isPerspective)
		return glm::perspective(glm::radians(fieldOfView),
			((viewport.endX - viewport.startX) * width) / ((viewport.endY - viewport.startY) * height),
			nearClipDistance, farClipDistance);
	return glm::mat4(1);
}

glm::mat4 gph::Camera::view() {
	return glm::lookAt(glm::vec3(gameObject->transform.position), lookAt(), glm::vec3(0, 1, 0));
}

void gph::Camera::setViewport(GLFWwindow* window) {
	int x, y, width, height;
	glfwGetWindowSize(window, &width, &height);
	x = int(std::round(viewport.startX * width));
	y = int(std::round(viewport.startY * height));
	width = int(std::round(viewport.endX * width));
	height = int(std::round(viewport.endY * height));
	glScissor(x, y, width, height);
	glEnable(GL_SCISSOR_TEST);
	glViewport(x, y, width, height);
}

boost::property_tree::ptree gph::Camera::serialize() {
	boost::property_tree::ptree node;
	node.put("isFixed", isFixed);
	node.put("isPerspective", isPerspective);
	node.put("moveSpeed", moveSpeed);
	node.put("rotationSpeed", rotationSpeed);
	node.put("nearClipDistance", nearClipDistance);
	node.put("farClipDistance", farClipDistance);
	node.put("fieldOfView", fieldOfView);
	node.put("yaw", yaw);
	node.put("pitch", pitch);
	node.add_child("viewport", viewport.serialize());
	return node;
}

void gph::Camera::deserialize(boost::property_tree::ptree node) {
	viewport = CameraViewport();
	viewport.deserialize(node.get_child("viewport"));
	isFixed = node.get<bool>("isFixed");
	isPerspective = node.get<bool>("isPerspective");
	moveSpeed = node.get<double>("moveSpeed");
	rotationSpeed = node.get<double>("rotationSpeed");
	nearClipDistance = node.get<float>("nearClipDistance");
	farClipDistance = node.get<float>("farClipDistance");
	fieldOfView = node.get<float>("fieldOfView");
	yaw = node.get<double>("yaw");
	pitch = node.get<double>("pitch");
}

boost::property_tree::ptree gph::CameraViewport::serialize() {
	boost::property_tree::ptree node;
	node.put("startX", startX);
	node.put("startY", startY);
	node.put("endX", endX);
	node.put("endY", endY);
	return node;
}

void gph::CameraViewport::deserialize(boost::property_tree::ptree node) {
	startX = node.get<float>("startX");
	startY = node.get<float>("startY");
	endX = node.get<float>("endX");
	endY = node.get<float>("endY");
}

gph::Camera* gph::Camera::instantiate() {
	Camera* cam = new Camera();
	cam->viewport = this->viewport;
	cam->isFixed = this->isFixed;
	cam->isPerspective = this->isPerspective;
	cam->moveSpeed = this->moveSpeed;
	cam->rotationSpeed = this->rotationSpeed;
	cam->nearClipDistance = this->nearClipDistance;
	cam->farClipDistance = this->farClipDistance;
	cam->fieldOfView = this->fieldOfView;
	cam->yaw = this->yaw;
	cam->pitch = this->pitch;
	return cam;
}

void gph::Camera::update() {
	for (int i = 0; i < totalCameraMovements; i++) {
		if (moveBuffer[i])
			moveCamera(i);
	}
	if (rotate) {
		rotateCamera(gpu->window);
	}
}

void gph::Camera::setType() {
	type = typeidToClassName(typeid(this).name());
}