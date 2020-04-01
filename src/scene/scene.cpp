#include "scene/scene.h"
#include "lib/log.h"
#include "components/phys_collider.h"
#include "components/solid_object.h"
#include "core/utils.h"

engine::Scene::Scene() : ctree(root_aabb()) {
}

engine::Scene::Scene(boost::property_tree::ptree node) : ctree(root_aabb()) {
	for (auto v : node.get_child("Game Object")) {
		addGameObject(new GameObject(v.second));
	}
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
	ullong id = getFirstAvailableMapIndex(gameObjects);
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
		scene.add_child("Game Object", g.second->serialize());
	}
	return node;
}