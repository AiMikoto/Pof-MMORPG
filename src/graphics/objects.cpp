#include "objects.h"
#include "transform.h"
#include "lib/log.h"
#include "scene.h"
#include "constants.h"
#include "variables.h"

namespace gph = graphics;

gph::GameObject::GameObject() {
	setup();
}

void gph::GameObject::setup() {
	if(this->id == -1)	generateID();
	gameObjects[this->id] = this;
}

gph::GameObject::~GameObject() {
	childrenIDs.clear();
}

gph::GameObject::GameObject(llong parentID) {
	this->parentID = parentID;
	setup();
}

gph::GameObject::GameObject(llong id, llong parentID) {
	this->id = id;
	this->parentID = parentID;
	setup();
}

gph::GameObject::GameObject(std::vector<llong> childrenIDs) {
	this->childrenIDs = childrenIDs;
	setup();
}

gph::GameObject::GameObject(llong parentID, std::vector<llong> childrenIDs) {
	this->childrenIDs = childrenIDs;
	this->parentID = parentID;
	setup();
}

gph::GameObject::GameObject(llong id, llong parentID, std::vector<llong> childrenIDs) {
	this->id = id;
	this->childrenIDs = childrenIDs;
	this->parentID = parentID;
	setup();
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

void gph::GameObject::generateID() {
	this->id = gameObjects.size();
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

void gph::GameObject::update(GLFWwindow* window) {}

std::string gph::GameObject::toJSON() {}

void gph::GameObject::fromJSON(std::string data) {}