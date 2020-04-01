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
		std::vector<uint> materialIDs;
		std::vector<std::pair<uint, uint>> renderLayers;

		MeshRenderer();
		~MeshRenderer();
		boost::property_tree::ptree serialize();
		MeshRenderer* deserialize(boost::property_tree::ptree node);
		MeshRenderer* instantiate();
		void draw(Camera* camera, GLFWwindow* window, uint materialID);
		void setup();
		void meshFilterRemoved();
		void modelRemoved();
	private:
		bool initialized = false;
		void cleanup();
		void setType();
	};
}