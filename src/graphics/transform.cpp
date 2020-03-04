#include "objects.h"
#include "transform.h"

namespace gph = graphics;

gph::Transform::Transform() { }

gph::Transform::~Transform() {
	delete gameObject;
}

gph::Transform::Transform(glm::vec3 position, glm::quat rotation, glm::vec3 scale) {
	this->position = position;
	this->rotation = rotation;
	this->scale = scale;
}

glm::vec3 gph::Transform::forward() {
	return rotation * glm::vec3(0, 0, -1);
}

glm::vec3 gph::Transform::up() {
	return rotation * glm::vec3(0, 1, 0);
}

glm::vec3 gph::Transform::right() {
	return rotation * glm::vec3(1, 0, 0);
}