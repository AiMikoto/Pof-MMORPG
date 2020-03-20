#include "scene/scene.h"
#include "lib/log.h"
#include <boost/property_tree/json_parser.hpp>

engine::Scene::Scene() {}

engine::Scene::~Scene() {
	for (auto g : gameObjects) {
		delete g;
	}
	gameObjects.clear();
}

void engine::Scene::update() {
	for (auto g : gameObjects) {
		g->update();
	}
}

void engine::Scene::sceneToJSON(std::string path) {
	boost::property_tree::ptree root = serialize();
	boost::property_tree::write_json(path, root);
}

void engine::Scene::sceneFromJSON(std::string data) {}

boost::property_tree::ptree engine::Scene::serialize() {
	boost::property_tree::ptree root, cNode, tNode, mNode, goNode;
	for (auto g : gameObjects) {
		root.add_child("Game Object", g->serialize());
	}
	return root;
}

void engine::Scene::deserialize(boost::property_tree::ptree node) {

}