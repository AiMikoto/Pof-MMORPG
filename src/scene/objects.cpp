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
		delete c;
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
	for (auto it : gameObject.children) {
		this->addGameObject(new GameObject(*it.second));
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
		addGameObject(std::stoull(c.first), new GameObject(c.second));
	}
}

engine::GameObject::GameObject(GameObject* parent) {
	this->parent = parent;
}

engine::GameObject::GameObject(std::vector<Component*> components) {
	this->components = components;
}

engine::GameObject::GameObject(GameObject* parent, std::vector<Component*> components) {
	this->parent = parent;
	this->components = components;
}

engine::Transform engine::GameObject::transformLocalToGlobal() {
	if(parent) {
		engine::Transform t = this->transform;
		engine::Transform pt = parent->transformLocalToGlobal();
		t.position += pt.position;
		t.rotation *= pt.rotation;
		t.scale *= pt.scale;
		return t;
	}
	return this->transform;
}

engine::GameObject *engine::GameObject::at(ullong index) {
	auto it = children.find(index);
	if(it != children.end()) {
		return it -> second;
	}
	throw std::logic_error("GameObject doesn't exist");
}

void engine::GameObject::update() {
	for (auto it : children)
		it.second->update();
}

ullong engine::GameObject::addGameObject(GameObject* child) {
	ullong id = getFirstAvailableMapIndex(this -> children);
	return addGameObject(id, child);
}

ullong engine::GameObject::addGameObject(ullong id, GameObject* child) {
	auto it = children.find(id);
	if(it != children.end()) { // new object takes priority
		delete it->second;
	}
	
	this->children[id] = child;
	child->parent = this;
	return id;
}

ullong engine::GameObject::addGameObject(ullong id, boost::property_tree::ptree node) {
	return addGameObject(id, new engine::GameObject(node.get_child("Game Object")));
}

void engine::GameObject::deleteGameObject(ullong id) {
	auto it = children.find(id);
	if(it != children.end()) {
		delete it -> second;
		children.erase(id);
	}
}

boost::property_tree::ptree engine::GameObject::serialize() {
	boost::property_tree::ptree node, componentsNode, childrenNode;
	node.put("name", name);
	node.put("tag", tag);
	for (auto c : components) {
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

