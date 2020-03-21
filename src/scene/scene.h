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
		~Scene();
		ullong addGameObject(GameObject* go);
		void update();
		void sceneToJSON(std::string path);
		void sceneFromJSON(std::string data);
		octree ctree;
	private:
		boost::property_tree::ptree serialize();
		void deserialize(boost::property_tree::ptree node);
	};
}
