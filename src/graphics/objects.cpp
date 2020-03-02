#include "objects.h"

namespace gph = graphics;

gph::GameObject::GameObject() {
	this->name = "";
	this->tag = "";
	this->id = generateID();
}

gph::GameObject::~GameObject() {
	for (auto c : children) {
		delete c;
	}
	children.clear();
}

gph::GameObject::GameObject(gph::GameObject* parent) {
	this->parent = parent;
}

gph::GameObject::GameObject(std::vector<GameObject*> children) {
	this->children = children;
}

gph::GameObject::GameObject(gph::GameObject* parent, std::vector<GameObject*> children) {
	this->children = children;
	this->parent = parent;
}

void gph::GameObject::add_child(GameObject * child) {
	this->children.push_back(child);
}

void gph::GameObject::add_children(std::vector<GameObject*> children) {
	this->children.insert(this->children.end(), children.begin(), children.end());
}

size_t gph::GameObject::generateID() {
	return 0;
}

gph::Transform::Transform() { }

gph::Transform::~Transform() { }

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

gph::Camera::~Camera() { }

gph::Camera::Camera(Transform* transform) { }
