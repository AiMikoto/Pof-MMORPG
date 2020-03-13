#include "objects.h"
#include "transform.h"
#include "lib/log.h"
#include "scene.h"
#include "constants.h"
#include "variables.h"
#include "utils.h"


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

void gph::GameObject::addParent(llong parentID) {
	this->parentID = parentID;
	this->updateTransform();
}

void gph::GameObject::addChild(llong childID) {
	bool found = false;
	for (auto i : childrenIDs) {
		if (i == childID) {
			found = true;
			break;
		}
	}
	if (!found) {
		this->childrenIDs.push_back(childID);
		gameObjects[childID]->parentID = this->id;
		gameObjects[childID]->updateTransform();
	}
}

void gph::GameObject::addChildren(std::vector<llong> childrenIDs) {
	for (auto i : childrenIDs) {
		this->addChild(i);
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

boost::property_tree::ptree gph::GameObject::serialize() {
	boost::property_tree::ptree node;
	node.put("name", name);
	node.put("tag", tag);
	node.put("id", id);
	node.put("parentID", parentID);
	node.put("childrenIDs", vectorToString(childrenIDs, ' '));
	node.add_child("transform", transform.serialize());
	return node;
}

void gph::GameObject::deserialize(boost::property_tree::ptree node) {
	gameObjects.erase(this->id);
	transform = Transform();
	transform.deserialize(node.get_child("transform"));
	name = node.get<std::string>("name");
	tag = node.get<std::string>("tag");
	id = node.get<llong>("id");
	parentID = node.get<llong>("parentID");
	std::string children = node.get<std::string>("children");
	childrenIDs = stringToVector<llong>(children, ' ');
}