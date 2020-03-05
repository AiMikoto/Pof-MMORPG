#include "objects.h"
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
	return rotation * glm::dvec3(0, 0, -1);
}

glm::dvec3 gph::Transform::up() {
	return rotation * glm::dvec3(0, 1, 0);
}

glm::dvec3 gph::Transform::right() {
	return rotation * glm::dvec3(1, 0, 0);
}