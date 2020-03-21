#pragma once
#include "scene/objects.h"
#include <vector>
#include <string>
#include <boost/property_tree/ptree.hpp>
#include "phys/octree.h"

namespace engine {
	class Scene {
	public:
		std::vector<GameObject*> gameObjects;

		Scene();
		~Scene();
		int add_GameObject(GameObject* go);
		void update();
		void sceneToJSON(std::string path);
		void sceneFromJSON(std::string data);
		octree ctree;
	private:
		boost::property_tree::ptree serialize();
		void deserialize(boost::property_tree::ptree node);
	};
}
