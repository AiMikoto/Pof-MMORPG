#pragma once
#include "components/component.h"
#include "scene/objects.h"
#include "components/camera.h"
#include "include/glad.h"
#include "include/glfw3.h"
#include "core/constants.h"

namespace engine {
	class MeshRenderer : public Component {
	public:
		std::string defaultMaterialPath;
		std::vector<std::string> materialsPaths;
		std::vector<std::pair<std::string, std::string>> renderLayers;

		MeshRenderer();
		MeshRenderer(const MeshRenderer& renderer);
		MeshRenderer(boost::property_tree::ptree node);
		~MeshRenderer();
		boost::property_tree::ptree serialize();
		void setup();
		void meshFilterRemoved();
		void modelRemoved();
	private:
		bool initialized = false;
		void cleanup();
		void setType();
	};
}