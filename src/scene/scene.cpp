#include "scene/scene.h"
#include "lib/log.h"
#include <boost/property_tree/json_parser.hpp>
#include "components/collider.h"
#include "components/physical.h"

engine::Scene::Scene() : ctree(root_aabb()) {
}

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

int engine::Scene::add_GameObject(GameObject* go) {
	int spot = gameObjects.size();
	gameObjects.push_back(go);
	if(go -> getComponent<physical>() -> collidable) {
		aabb caabb = go -> getComponent<collider>() -> to_aabb();
		ctree.insert(spot, caabb);
	}
	return spot;
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
