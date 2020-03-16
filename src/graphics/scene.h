#pragma once
#include "graphics_files.h"
#include "objects.h"
#include "transform.h"
#include "shader.h"
#include "camera.h"
#include "light.h"
#include "model.h"
#include <vector>
#include <string>
#include <boost/property_tree/ptree.hpp>

namespace graphics {
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