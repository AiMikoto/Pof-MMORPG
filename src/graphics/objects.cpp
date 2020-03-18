#include "objects.h"
#include "constants.h"
#include "component.h"
#include "mesh.h"

namespace gph = graphics;

gph::GameObject::GameObject() {}

gph::GameObject::~GameObject() {
	for (auto c : components) {
		delete c;
	}
	components.clear();
	for (auto c : children) {
		delete c;
	}
	children.clear();
}

gph::GameObject::GameObject(GameObject* parent) {
	this->parent = parent;
}

gph::GameObject::GameObject(std::vector<GameObject*> children) {
	this->children = children;
}

gph::GameObject::GameObject(std::vector<Component*> components) {
	this->components = components;
}

gph::GameObject::GameObject(GameObject* parent, std::vector<GameObject*> children) {
	this->parent = parent;
	this->children = children;
}

gph::GameObject::GameObject(GameObject* parent, std::vector<Component*> components) {
	this->parent = parent;
	this->components = components;
}

gph::GameObject::GameObject(std::vector<GameObject*> children, std::vector<Component*> components) {
	this->children = children;
	this->components = components;
}

gph::GameObject::GameObject(GameObject* parent, std::vector<GameObject*> children, std::vector<Component*> components) {
	this->parent = parent;
	this->children = children;
	this->components = components;
}

void gph::GameObject::update() {}

gph::GameObject* gph::GameObject::instantiate() {
	GameObject* gameObject = new GameObject(this->parent);
	gameObject->transform = this->transform;
	for (auto c : components) {
		if (static_cast<MeshLoader*>(c)->type == typeid(MeshLoader).name())
			gameObject->addComponent(static_cast<MeshLoader*>(c)->instantiate());
		else if (static_cast<MeshRenderer*>(c)->type == typeid(MeshRenderer).name())
			gameObject->addComponent(static_cast<MeshRenderer*>(c)->instantiate());
		else if (static_cast<Camera*>(c)->type == typeid(Camera).name())
			gameObject->addComponent(static_cast<Camera*>(c)->instantiate());
	}
	gameObject->addComponents(components);
	for (auto c : children) {
		gameObject->addChild(c->instantiate());
	}
}

void gph::GameObject::addChild(GameObject* child) {
	bool found = false;
	for (auto i : this->children) {
		if (i == child) {
			found = true;
			break;
		}
	}
	if (!found) {
		this->children.push_back(child);
		child->parent = this;
	}
}

void gph::GameObject::addChildren(std::vector<GameObject*> children) {
	for (auto i : children) {
		this->addChild(i);
	}
}

void gph::GameObject::addComponent(Component* component) {
	bool found = false;
	for (auto i : this->components) {
		if (i == component) {
			found = true;
			break;
		}
	}
	if (!found) {
		this->components.push_back(component);
		component->gameObject = this;
	}
}

void gph::GameObject::addComponents(std::vector<Component*> components) {
	for (auto i : components) {
		this->addComponent(i);
	}
}

boost::property_tree::ptree gph::GameObject::serialize() {
	boost::property_tree::ptree node;
	node.put("name", name);
	node.put("tag", tag);
	for (auto c : components) {
		node.add_child("component", c->serialize());
	}
	for (auto c : children) {
		node.add_child("game object", c->serialize());
	}	
	node.add_child("transform", transform.serialize());
	return node;
}

void gph::GameObject::deserialize(boost::property_tree::ptree node) {
	
}