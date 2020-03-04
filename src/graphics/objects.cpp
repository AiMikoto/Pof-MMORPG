#include "objects.h"
#include "transform.h"

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
	delete transform;
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
