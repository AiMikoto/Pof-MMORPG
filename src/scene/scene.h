#pragma once
#include "scene/objects.h"
#include <vector>
#include <string>
#include <boost/property_tree/ptree.hpp>

namespace engine {
	class Scene {
	public:
		std::vector<GameObject*> gameObjects;

		Scene();
		~Scene();
		void update();
		void sceneToJSON(std::string path);
		void sceneFromJSON(std::string data);
	private:
		boost::property_tree::ptree serialize();
		void deserialize(boost::property_tree::ptree node);
	};
}