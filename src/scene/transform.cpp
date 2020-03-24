#include "scene/transform.h"
#include "core/utils.h"

engine::Transform::Transform() {
	this->position = glm::dvec3(0, 0, 0);
	this->rotation = glm::quat(1, 0, 0, 0);
	this->scale = glm::vec3(1, 1, 1);
}

engine::Transform::~Transform() { }

engine::Transform::Transform(glm::dvec3 position, glm::dquat rotation, glm::dvec3 scale) {
	this->position = position;
	this->rotation = rotation;
	this->scale = scale;
}

glm::dvec3 engine::Transform::forward() {
	return glm::normalize(rotation * glm::dvec3(0, 0, -1));
}

glm::dvec3 engine::Transform::up() {
	return glm::normalize(rotation * glm::dvec3(0, 1, 0));
}

glm::dvec3 engine::Transform::right() {
	return glm::normalize(rotation * glm::dvec3(1, 0, 0));
}

void engine::Transform::rotateTo(glm::dvec3 rotationAngles) {
	std::pair<double, glm::dvec3> angleAxis = anglesToAngleAxis(rotationAngles);
	rotateTo(angleAxis.first, glm::normalize(angleAxis.second));
}

void engine::Transform::rotateTo(double angle, glm::dvec3 axes) {
	rotation = glm::angleAxis(angle, glm::normalize(axes));
}

void engine::Transform::rotateBy(glm::dvec3 rotationAngles) {
	std::pair<double, glm::dvec3> angleAxis = anglesToAngleAxis(rotationAngles);
	rotateBy(angleAxis.first, glm::normalize(angleAxis.second));
}

void engine::Transform::rotateBy(double angle, glm::dvec3 axes) {
	glm::dquat rotation = glm::angleAxis(angle, glm::normalize(axes));
	this->rotation *= rotation;
}

void engine::Transform::rotateBy(glm::dquat rotation) {
	this->rotation *= rotation;
}

std::pair<double, glm::dvec3> engine::Transform::anglesToAngleAxis(glm::dvec3 rotationAngles) {
	double angle = highestCommonDenominator(rotationAngles.x, rotationAngles.y);
	angle = highestCommonDenominator(angle, rotationAngles.z);
	glm::dvec3 axes = glm::dvec3(rotationAngles.x / angle, rotationAngles.y / angle, rotationAngles.z / angle);
	return std::pair<double, glm::dvec3>(angle, axes);
}

glm::mat4 engine::Transform::translationMatrix() {
	return glm::translate(glm::mat4(1), glm::vec3(this->position));
}

glm::mat4 engine::Transform::rotationMatrix() {
	return glm::mat4_cast(glm::quat(this->rotation));
}

glm::mat4 engine::Transform::scaleMatrix() {
	return glm::scale(glm::mat4(1), glm::vec3(this->scale));
}

glm::mat4 engine::Transform::model() {
	return translationMatrix() * rotationMatrix() * scaleMatrix();
}

boost::property_tree::ptree engine::Transform::serialize() {
	boost::property_tree::ptree node;
	node.add_child("position", dvec3serializer(position));
	node.add_child("rotation", dvec4serializer(glm::dvec4(rotation.x, rotation.y, rotation.z, rotation.w)));
	node.add_child("scale", dvec3serializer(scale));
	return node;
}

void engine::Transform::deserialize(boost::property_tree::ptree node) {
	
}