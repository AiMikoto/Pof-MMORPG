#include "scene/objects.h"
#include "core/constants.h"
#include "components/component.h"
#include "components/camera.h"
#include "components/light.h"
#include "components/meshFilter.h"
#include "components/meshRenderer.h"
#include "components/phys_collider.h"
#include "components/solid_object.h"
#include "lib/log.h"

engine::GameObject::GameObject() {
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

engine::GameObject::GameObject(const GameObject& gameObject) {
	this->parent = NULL;
	this->transform = gameObject.transform;
	for (auto c : gameObject.components) {
		if (c->type == typeid(Camera).name())
			this->addComponent(new Camera(*static_cast<Camera*>(c)));
		//if (c->type == typeid(Light).name())
			//this->addComponent(new Light(*static_cast<Light*>(c)));
		if (c->type == typeid(MeshFilter).name())
			this->addComponent(new MeshFilter(*static_cast<MeshFilter*>(c)));
		if (c->type == typeid(MeshRenderer).name())
			this->addComponent(new MeshRenderer(*static_cast<MeshRenderer*>(c)));
	}
	for (auto c : gameObject.children) {
		this->addChild(new GameObject(*c));
	}
	id = gameObject.id;
	name = gameObject.name;
	tag = gameObject.tag;
}

engine::GameObject::GameObject(boost::property_tree::ptree node) {
	try{
		id = node.get<int>("id");
	} catch (std::exception &e) {
		id = 0;
	}
	name = node.get<std::string>("name");
	tag = node.get<std::string>("tag");
	for (auto c : node.get_child("Components")) {
		boost::property_tree::ptree t;
		t.put_child(c.first, c.second);
		constructComponent(t);
	}
	transform = Transform(node.get_child("Transform"));
	for (auto c : node.get_child("Children")) {
		addChild(new GameObject(c.second));
	}
}

engine::GameObject::GameObject(GameObject* parent) {
	this->parent = parent;
}

engine::GameObject::GameObject(std::vector<GameObject*> children) {
	this->children = children;
}

engine::GameObject::GameObject(std::vector<Component*> components) {
	this->components = components;
}

engine::GameObject::GameObject(GameObject* parent, std::vector<GameObject*> children) {
	this->parent = parent;
	this->children = children;
}

engine::GameObject::GameObject(GameObject* parent, std::vector<Component*> components) {
	this->parent = parent;
	this->components = components;
}

engine::GameObject::GameObject(std::vector<GameObject*> children, std::vector<Component*> components) {
	this->children = children;
	this->components = components;
}

engine::GameObject::GameObject(GameObject* parent, std::vector<GameObject*> children, std::vector<Component*> components) {
	this->parent = parent;
	this->children = children;
	this->components = components;
}

void engine::GameObject::update() {
	for (auto c : children)
		c->update();
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

boost::property_tree::ptree engine::GameObject::serialize() {
	boost::property_tree::ptree node, componentsNode, childrenNode;
	node.put("name", name);
	node.put("tag", tag);
	node.put("id", id);
	for (auto c : components) {
		componentsNode.add_child(c->name, c->serialize());
	}
	for (auto c : children) {
		childrenNode.add_child("GameObject", c->serialize());
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

