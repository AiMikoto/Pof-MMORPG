#include "objects.h"
#include "transform.h"
#include "lib/log.h"

namespace gph = graphics;

std::map<size_t, gph::GameObject*> gph::gameObjects;

gph::GameObject::GameObject() {
	setup();
}

void gph::GameObject::setup() {
	this->name = "";
	this->tag = "";
	this->id = generateID();
	gameObjects[this->id] = this;
}

gph::GameObject::~GameObject() {
  BOOST_LOG_TRIVIAL(trace) << "deleting children";
	for (auto c : children) {
		delete c;
	}
  BOOST_LOG_TRIVIAL(trace) << "clearing children";
	children.clear();
}

gph::GameObject::GameObject(gph::GameObject* parent) {
	setup();
	this->parent = parent;
}

gph::GameObject::GameObject(std::vector<GameObject*> children) {
	setup();
	this->children = children;
}

gph::GameObject::GameObject(gph::GameObject* parent, std::vector<GameObject*> children) {
	setup();
	this->children = children;
	this->parent = parent;
}

void gph::GameObject::add_child(GameObject * child) {
	this->children.push_back(child);
}

void gph::GameObject::add_children(std::vector<GameObject*> children) {
	this->children.insert(this->children.end(), children.begin(), children.end());
}

void gph::GameObject::draw() {}

size_t gph::GameObject::generateID() {
	return 0;
}