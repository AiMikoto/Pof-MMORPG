#include "objects.h"

namespace gph = graphics;

gph::Quaternion::Quaternion() {}

gph::Quaternion::Quaternion(glm::vec4 values) {
	this->values = values;
}

glm::vec3 gph::Quaternion::eulerAngles() {
	return glm::vec3();
}

glm::vec3 gph::Quaternion::quaternionToEuler(glm::vec4 values, bool assignValues = false) {
	if (assignValues) this->values = values;
	return glm::vec3();
}

glm::vec4 gph::Quaternion::eulerToQuaternion(glm::vec3 values, bool assignValues = false) {
	if (assignValues) this->values = glm::vec4();
	return glm::vec4();
}

glm::vec3 graphics::Quaternion::operator * (glm::vec3 point) {
	float num1 = values.x * 2.0f;
	float num2 = values.y * 2.0f;
	float num3 = values.z * 2.0f;
	float num4 = values.x * num1;
	float num5 = values.y * num2;
	float num6 = values.z * num3;
	float num7 = values.x * num2;
	float num8 = values.x * num3;
	float num9 = values.y * num3;
	float num10 = values.w * num1;
	float num11 = values.w * num2;
	float num12 = values.w * num3;
	glm::vec3 res;
	res.x = (float)((1.0 - ((double)num5 + (double)num6)) * (double)point.x +
		((double)num7 - (double)num12) * (double)point.y +
		((double)num8 + (double)num11) * (double)point.z);
	res.y = (float)(((double)num7 + (double)num12) * (double)point.x +
		(1.0 - ((double)num4 + (double)num6)) * (double)point.y +
		((double)num9 - (double)num10) * (double)point.z);
	res.z = (float)(((double)num8 - (double)num11) * (double)point.x +
		((double)num9 + (double)num10) * (double)point.y +
		(1.0 - ((double)num4 + (double)num5)) * (double)point.z);
	return res;
}

gph::GameObject::GameObject() { }

gph::GameObject::GameObject(gph::Transform* transform) {
	this->transform = transform;
}

gph::GameObject::GameObject(std::vector<GameObject*> children) {
	this->children = children;
}

gph::GameObject::GameObject(std::vector<GameObject*> children, gph::Transform* transform) {
	this->children = children;
	this->transform = transform;
}

void gph::GameObject::add_child(GameObject * child) {
	this->children.push_back(child);
}

void gph::GameObject::add_children(std::vector<GameObject*> children) {
	this->children.insert(this->children.end(), children.begin(), children.end());
}


gph::Transform::Transform() { }

gph::Transform::Transform(glm::vec3 position, glm::quat rotation, glm::vec3 scale) {
	this->position = position;
	this->rotation = rotation;
	this->scale = scale;
}

glm::vec3 gph::Transform::forward() {
	return rotation * glm::vec3(0,0,-1);
}

glm::vec3 gph::Transform::up() {
	return rotation * glm::vec3(0, 1, 0);
}

glm::vec3 gph::Transform::right() {
	return rotation * glm::vec3(1, 0, 0);
}

gph::Camera::Camera() { }

gph::Camera::Camera(Transform* transform) { }
