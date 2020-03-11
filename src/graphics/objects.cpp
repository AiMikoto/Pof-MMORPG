#include "objects.h"
#include "transform.h"
#include "lib/log.h"

namespace gph = graphics;

std::map<llong, gph::GameObject*> gph::gameObjects;

gph::GameObject::GameObject() {
	setup();
}

void gph::GameObject::setup() {
	this->parentID = -1;
	this->name = "";
	this->tag = "";
	generateID();
}

gph::GameObject::~GameObject() {
	BOOST_LOG_TRIVIAL(trace) << "clearing children";
	childrenIDs.clear();
}

gph::GameObject::GameObject(llong parentID) {
	setup();
	this->parentID = parentID;
}

gph::GameObject::GameObject(std::vector<llong> childrenIDs) {
	setup();
	this->childrenIDs = childrenIDs;
}

gph::GameObject::GameObject(llong parentID, std::vector<llong> childrenIDs) {
	setup();
	this->childrenIDs = childrenIDs;
	this->parentID = parentID;
}

void gph::GameObject::add_child(llong child) {
	this->childrenIDs.push_back(child);
	gameObjects[child]->parentID = this->id;
}

void gph::GameObject::add_children(std::vector<llong> childrenIDs) {
	for (auto i : childrenIDs) {
		this->add_child(i);
	}
}

void gph::GameObject::draw(Shader* shader, GameObject* camera, GLFWwindow* window) {}

void gph::GameObject::generateID() {
	this->id = gameObjects.size();
	gameObjects[this->id] = this;
}

void gph::GameObject::updateTransform() {
	if (parentID != -1) {
		transform.position += gameObjects[parentID]->transform.position;
		transform.rotation *= gameObjects[parentID]->transform.rotation;
		transform.scale *= gameObjects[parentID]->transform.scale;
	}
	for (auto i : childrenIDs) {
		gameObjects[i]->updateTransform();
	}
}