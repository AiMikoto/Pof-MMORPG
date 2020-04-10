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
	class Scene : public GameObject{
	public:
		octree ctree;
		long long generation;
		// tag is unique and not saved
		std::string tag;
		map_t map;
		bool saveOnExit = false;

		Scene();
		Scene(boost::property_tree::ptree node);
		~Scene();
		void regenerateCtree();
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
