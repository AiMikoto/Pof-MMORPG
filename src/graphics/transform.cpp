#include "transform.h"

namespace gph = graphics;

gph::Transform::Transform() { }

gph::Transform::~Transform() { }

gph::Transform::Transform(glm::dvec3 position, glm::dquat rotation, glm::dvec3 scale) {
	this->position = position;
	this->rotation = rotation;
	this->scale = scale;
}

glm::dvec3 gph::Transform::forward() {
	return glm::normalize(rotation * glm::dvec3(0, 0, -1));
}

glm::dvec3 gph::Transform::up() {
	return glm::normalize(rotation * glm::dvec3(0, 1, 0));
}

glm::dvec3 gph::Transform::right() {
	return glm::normalize(rotation * glm::dvec3(1, 0, 0));
}

glm::mat4 gph::Transform::model() {
	glm::mat4 translation = glm::translate(glm::mat4(1), glm::vec3(this->position));
	glm::mat4 rotation = glm::mat4_cast(glm::quat(rotation));
	glm::mat4 scale = glm::scale(glm::mat4(1), glm::vec3(this->scale));
	return translation * rotation * scale;
}