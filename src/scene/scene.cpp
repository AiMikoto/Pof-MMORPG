#include "scene/scene.h"
#include "lib/log.h"
#include "components/phys_collider.h"
#include "components/solid_object.h"
#include "core/utils.h"
#include <boost/property_tree/json_parser.hpp>
#include <sstream>

engine::Scene::Scene() : ctree(root_aabb()) {
}

engine::Scene::~Scene() {
	for (auto g : gameObjects) {
		delete g.second;
	}
	gameObjects.clear();
}

void engine::Scene::update() {
	for (auto g : gameObjects) {
		g.second->update();
	}
}

ullong engine::Scene::addGameObject(GameObject* go) {
	ullong id = (go->id == 0) ? getFirstAvailableMapIndex(gameObjects) : go->id;
	gameObjects[id] = go;
	go->id = id;
	collider* phys = go->getComponent<physical_collider>();
	if (phys && !go->hasComponent<solid_object>()) {
		aabb caabb = phys->to_aabb();
		ctree.insert(int(id), caabb);
	}
	return id;
}

ullong engine::Scene::addGameObject(boost::property_tree::ptree node) {
	return addGameObject(new GameObject(node.get_child("Game Object")));
}

boost::property_tree::ptree engine::Scene::serialize() {
	boost::property_tree::ptree node, scene;
	for (auto g : gameObjects) {
		scene.add_child("GameObject", g.second->serialize());
	}
	node.add_child("Scene", scene);
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
	for (auto go : node.get_child("Scene")) {
		addGameObject(new GameObject(go.second));
	}
}