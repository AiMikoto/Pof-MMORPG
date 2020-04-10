#include "scene/objects.h"
#include "core/constants.h"
#include "components/component.h"
#include "components/camera.h"
#include "components/light.h"
#include "components/meshFilter.h"
#include "components/meshRenderer.h"
#include "components/phys_collider.h"
#include "components/solid_object.h"
#include "core/utils.h"
#include "lib/log.h"

engine::GameObject::GameObject() {
}

engine::GameObject::~GameObject() {
	for (auto c : components) {
		delete c.second;
	}
	components.clear();
	for (auto c : children) {
		delete c.second;
	}
	children.clear();
}

engine::GameObject::GameObject(const GameObject& gameObject) {
	this->parent = NULL;
	this->transform = gameObject.transform;
	for (auto it : gameObject.components) {
		engine::Component *c = it.second;
		if (c->type == typeid(Camera).name())
			this->addComponent(new Camera(*static_cast<Camera*>(c)));
		//if (c->type == typeid(Light).name())
			//this->addComponent(new Light(*static_cast<Light*>(c)));
		if (c->type == typeid(MeshFilter).name())
			this->addComponent(new MeshFilter(*static_cast<MeshFilter*>(c)));
		if (c->type == typeid(MeshRenderer).name())
			this->addComponent(new MeshRenderer(*static_cast<MeshRenderer*>(c)));
	}
	for (auto it : gameObject.children) {
		this->addChild(new GameObject(*it.second));
	}
	name = gameObject.name;
	tag = gameObject.tag;
}

engine::GameObject::GameObject(boost::property_tree::ptree node) {
	name = node.get<std::string>("name");
	tag = node.get<std::string>("tag");
	for (auto c : node.get_child("Components")) {
		boost::property_tree::ptree t;
		t.put_child(c.first, c.second);
		constructComponent(t);
	}
	transform = Transform(node.get_child("Transform"));
	for (auto c : node.get_child("Children")) {
		addChild(std::stoi(c.first), new GameObject(c.second));
	}
}

engine::GameObject::GameObject(GameObject* parent) {
	this->parent = parent;
}

engine::GameObject::GameObject(std::map<ullong, GameObject*> children) {
	this->children = children;
}

engine::GameObject::GameObject(std::map<ullong, Component*> components) {
	this->components = components;
}

engine::GameObject::GameObject(GameObject* parent, std::map<ullong, GameObject*> children) {
	this->parent = parent;
	this->children = children;
}

engine::GameObject::GameObject(GameObject* parent, std::map<ullong, Component*> components) {
	this->parent = parent;
	this->components = components;
}

engine::GameObject::GameObject(std::map<ullong, GameObject*> children, std::map<ullong, Component*> components) {
	this->children = children;
	this->components = components;
}

engine::GameObject::GameObject(GameObject* parent, std::map<ullong, GameObject*> children, std::map<ullong, Component*> components) {
	this->parent = parent;
	this->children = children;
	this->components = components;
}

engine::GameObject engine::GameObject::at(ullong index) {
	return children[index];
}

void engine::GameObject::update() {
	for (auto it : children)
		it.second->update();
}

void engine::GameObject::addChild(GameObject* child) {
	ullong id = getFirstAvailableMapIndex(this -> children);
	addChild(id, child);
}

void engine::GameObject::addChild(ullong id, GameObject* child) {
	bool found = false;
	for (auto it : this->children) {
		if (it.second == child) {
			found = true;
			break;
		}
	}
	if (!found) {
		this->children[id] = child;
		child->parent = this;
	}
}

void engine::GameObject::addChildren(std::map<ullong, GameObject*> children) {
	for (auto i : children) {
		this->addChild(i.second);
	}
}

boost::property_tree::ptree engine::GameObject::serialize() {
	boost::property_tree::ptree node, componentsNode, childrenNode;
	node.put("name", name);
	node.put("tag", tag);
	for (auto it : components) {
		engine::Component *c = it.second;
		componentsNode.add_child(c->name, c->serialize());
	}
	for (auto it : children) {
		engine::GameObject *c = it.second;
		childrenNode.add_child(std::to_string(it.first), c->serialize());
	}	
	node.add_child("Transform", transform.serialize());
	node.add_child("Components", componentsNode);
	node.add_child("Children", childrenNode);
	return node;
}

bool engine::GameObject::constructComponent(boost::property_tree::ptree node) {
	for (auto c : node) {
		if (c.first == "Camera") {
			return addComponent(new Camera(c.second));
		}
		if (c.first == "MeshFilter") {
			return addComponent(new MeshFilter(c.second));
		}
		if (c.first == "MeshRenderer") {
			return addComponent(new MeshRenderer(c.second));
		}
		if (c.first == "physical_collider") {
			return addComponent(new physical_collider(c.second));
		}
		if (c.first == "solid_object") {
			return addComponent(new solid_object(c.second));
		}
	}
	return false;
}

