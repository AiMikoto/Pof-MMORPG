#include "scene/scene.h"
#include "lib/log.h"
#include "components/phys_collider.h"
#include "components/solid_object.h"
#include "phys/aabb.h"
#include "core/utils.h"
#include <boost/property_tree/json_parser.hpp>
#include <sstream>

engine::Scene::Scene() : GameObject(), ctree(root_aabb()) {
	generation = 0;
}

engine::Scene::Scene(boost::property_tree::ptree node) : GameObject(), ctree(root_aabb()) {
	deserialize(node);
}

engine::Scene::~Scene() {
}

void engine::Scene::regenerateCtree() {
	for (auto g : children) {
		GameObject *go = g.second;
		collider* phys = go->getComponent<physical_collider>();
		if (phys) {
			aabb caabb = phys->to_aabb();
			ctree.insert(g.first, caabb);
		}
		if(go->hasComponent<solid_object>()) {
			ctree.erase(g.first);
		}
	}
}

boost::property_tree::ptree engine::Scene::serialize() {
	boost::property_tree::ptree node, scene;
	for (auto g : children) {
		scene.add_child(std::to_string(g.first), g.second->serialize());
	}
	node.add_child("Scene", scene);
	node.put("generation", generation);
	node.put("tag", tag);
	return node;
}

std::string engine::Scene::toJSON() {
	std::stringstream ss;
	boost::property_tree::write_json(ss, serialize());
	return ss.str();
}

void engine::Scene::fromJSON(std::string data) {
	boost::property_tree::ptree root;
	std::stringstream ss;
	ss << data;
	boost::property_tree::read_json(ss, root);
	deserialize(root);
}

void engine::Scene::writeToFile(std::string path) {
	boost::property_tree::write_json(path, serialize());
}

void engine::Scene::readFromFile(std::string path) {
	boost::property_tree::ptree root;
	boost::property_tree::read_json(path, root);
	deserialize(root);
}

void engine::Scene::deserialize(boost::property_tree::ptree node) {
	try {
		generation = node.get<long long>("generation");
	} catch(std::exception &e) {
		generation = 0;
	}
	try {
		tag = node.get<std::string>("tag");
	} catch(std::exception &e) {
		tag = "";
	}
	for (auto go : node.get_child("Scene")) {
		addGameObject(std::stoull(go.first), new GameObject(go.second));
	}
}
