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
		ullong addGameObject(boost::property_tree::ptree node);
		void update();
		boost::property_tree::ptree serialize();
		octree ctree;		
	};
}
