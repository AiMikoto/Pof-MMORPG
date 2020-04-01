#pragma once
#include "scene/objects.h"
#include <vector>
#include <string>
#include <map>
#include <boost/property_tree/ptree.hpp>
#include "phys/octree.h"
#include "core/constants.h"

namespace engine {
	class Scene {
	public:
		std::map<ullong, GameObject*> gameObjects;

		Scene();
		Scene(boost::property_tree::ptree node);
		~Scene();
		ullong addGameObject(GameObject* go);
		void update();
		void sceneToJSON(std::string path);
		void sceneFromJSON(std::string data);
		octree ctree;
		boost::property_tree::ptree serialize();
	private:
		void deserialize(boost::property_tree::ptree node);
	};
}
