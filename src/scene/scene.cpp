#include "scene/scene.h"
#include "lib/log.h"
#include <boost/property_tree/json_parser.hpp>
#include "components/collider.h"
#include "components/physical.h"

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
	ullong id = ullong(gameObjects.size() + 1);
	gameObjects[id] = go;
	go->id = id;
	physical* phys = go->getComponent<physical>();
	if( phys!= NULL) {
		if (phys->collidable) {
			aabb caabb = go->getComponent<collider>()->to_aabb();
			ctree.insert(int(id), caabb);
		}
	}
	return id;
}

void engine::Scene::sceneToJSON(std::string path) {
	boost::property_tree::ptree root = serialize();
	boost::property_tree::write_json(path, root);
}

void engine::Scene::sceneFromJSON(std::string data) {}

boost::property_tree::ptree engine::Scene::serialize() {
	boost::property_tree::ptree root, cNode, tNode, mNode, goNode;
	for (auto g : gameObjects) {
		root.add_child("Game Object", g.second->serialize());
	}
	return root;
}

void engine::Scene::deserialize(boost::property_tree::ptree node) {

}
