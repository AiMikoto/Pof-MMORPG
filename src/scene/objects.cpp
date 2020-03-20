#include "scene/objects.h"
#include "core/constants.h"
#include "components/component.h"

engine::GameObject::GameObject() {
	initialise();
}

engine::GameObject::~GameObject() {
	for (auto c : components) {
		delete c;
	}
	components.clear();
	for (auto c : children) {
		delete c;
	}
	children.clear();
}

engine::GameObject::GameObject(GameObject* parent) {
	this->parent = parent;
	initialise();
}

engine::GameObject::GameObject(std::vector<GameObject*> children) {
	this->children = children;
	initialise();
}

engine::GameObject::GameObject(std::vector<Component*> components) {
	this->components = components;
	initialise();
}

engine::GameObject::GameObject(GameObject* parent, std::vector<GameObject*> children) {
	this->parent = parent;
	this->children = children;
	initialise();
}

engine::GameObject::GameObject(GameObject* parent, std::vector<Component*> components) {
	this->parent = parent;
	this->components = components;
	initialise();
}

engine::GameObject::GameObject(std::vector<GameObject*> children, std::vector<Component*> components) {
	this->children = children;
	this->components = components;
	initialise();
}

engine::GameObject::GameObject(GameObject* parent, std::vector<GameObject*> children, std::vector<Component*> components) {
	this->parent = parent;
	this->children = children;
	this->components = components;
	initialise();
}

void engine::GameObject::initialise()
{
	velocity = {0, 0, 0};
	force_acc = {0, 0, 0};
	m = 1;
	im = 1;
	collidable = false;
	collides = false;
	movable = false;
}

void engine::GameObject::tick(double delta)
{
	velocity += im * force_acc * delta;
	force_acc = {0, 0, 0};
	transform.position += velocity * delta;
}

void engine::GameObject::add_force(glm::dvec3 force)
{
  this -> force_acc += force;
}

void engine::GameObject::update() {
	for (auto c : children)
		c->update();
}

engine::GameObject* engine::GameObject::instantiate() {
	GameObject* gameObject = new GameObject(this->parent);
	gameObject->transform = this->transform;
	for (auto c : components) {
		gameObject->addComponent(c->instantiate());
	}
	for (auto c : children) {
		gameObject->addChild(c->instantiate());
	}
}

void engine::GameObject::addChild(GameObject* child) {
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

void engine::GameObject::addChildren(std::vector<GameObject*> children) {
	for (auto i : children) {
		this->addChild(i);
	}
}

void engine::GameObject::addComponent(Component* component) {
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

void engine::GameObject::addComponents(std::vector<Component*> components) {
	for (auto i : components) {
		this->addComponent(i);
	}
}

boost::property_tree::ptree engine::GameObject::serialize() {
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

void engine::GameObject::deserialize(boost::property_tree::ptree node) {
	
}
