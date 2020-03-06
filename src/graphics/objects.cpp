#include "objects.h"
#include "transform.h"
#include "lib/log.h"

namespace gph = graphics;

gph::GameObject::GameObject() {
	this->name = "";
	this->tag = "";
	this->id = generateID();
}

gph::GameObject::~GameObject() {
  BOOST_LOG_TRIVIAL(trace) << "deleting children";
	for (auto c : children) {
		delete c;
	}
  BOOST_LOG_TRIVIAL(trace) << "clearing children";
	children.clear();
  BOOST_LOG_TRIVIAL(trace) << "clearing transform";
	if(transform != NULL)
		delete transform;
}

gph::GameObject::GameObject(gph::GameObject* parent) {
  BOOST_LOG_TRIVIAL(trace) << "unused";
	this->parent = parent;
}

gph::GameObject::GameObject(std::vector<GameObject*> children) {
  BOOST_LOG_TRIVIAL(trace) << "unused";
	this->children = children;
}

gph::GameObject::GameObject(gph::GameObject* parent, std::vector<GameObject*> children) {
  BOOST_LOG_TRIVIAL(trace) << "unused";
	this->children = children;
	this->parent = parent;
}

void gph::GameObject::add_child(GameObject * child) {
  BOOST_LOG_TRIVIAL(trace) << "unused";
	this->children.push_back(child);
}

void gph::GameObject::add_children(std::vector<GameObject*> children) {
  BOOST_LOG_TRIVIAL(trace) << "unused";
	this->children.insert(this->children.end(), children.begin(), children.end());
}

size_t gph::GameObject::generateID() {
	return 0;
}
