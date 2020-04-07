#pragma once
#include "scene/objects.h"
#include <vector>
#include <string>
#include <map>
#include <boost/property_tree/ptree.hpp>
#include "phys/octree.h"
#include "core/constants.h"
#include "include/maps.h"

namespace engine {
	class Scene {
	public:
		std::map<ullong, GameObject*> gameObjects;
		octree ctree;
		long long generation;
		// tag is unique and not saved
		std::string tag;
		map_t map;

		Scene();
		Scene(boost::property_tree::ptree node);
		~Scene();
		ullong addGameObject(GameObject* go);
		ullong addGameObject(boost::property_tree::ptree node);
		void deleteGameObject(ullong id);
		void update();
		void sceneToJSON(std::string path);
		void sceneFromJSON(std::string data);
		boost::property_tree::ptree serialize();
		void deserialize(boost::property_tree::ptree node);
		std::string toJSON();
		void fromJSON(std::string data);
		void writeToFile(std::string path);
		void readFromFile(std::string path);
	private:
	};
}
