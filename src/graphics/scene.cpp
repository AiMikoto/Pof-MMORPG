#include "scene.h"
#include "gl_functions.h"
#include "lib/log.h"
#include <boost/property_tree/json_parser.hpp>

namespace gph = graphics;

gph::Scene::Scene() {}

gph::Scene::~Scene() {
	for (auto g : gameObjects) {
		delete g;
	}
	gameObjects.clear();
}

void gph::Scene::update() {
	for (auto g : gameObjects) {
		g->update();
	}
}

void gph::Scene::sceneToJSON(std::string path) {
	boost::property_tree::ptree root = serialize();
	boost::property_tree::write_json(path, root);
}

void gph::Scene::sceneFromJSON(std::string data) {}

boost::property_tree::ptree gph::Scene::serialize() {
	boost::property_tree::ptree root, cNode, tNode, mNode, goNode;
	for (auto g : gameObjects) {
		root.add_child("Game Object", g->serialize());
	}
	return root;
}

void deserialize(boost::property_tree::ptree node) {

}