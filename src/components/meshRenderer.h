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
		std::vector<uint> meshIDs;
		std::vector<uint> materialIDs;

		MeshRenderer();
		~MeshRenderer();
		boost::property_tree::ptree serialize();
		void deserialize(boost::property_tree::ptree node);
		MeshRenderer* instantiate();
		void draw(Camera* camera, GLFWwindow* window, uint materialID);
		virtual void setup();
		void meshFilterRemoved(uint meshID);
	private:
		std::vector<GLuint> vertexArrayID, vertexBufferID, elementsBufferID, outlineIndicesBufferID;
		bool initialized = false;
		void glContextSetup();
		void bindBuffers();
		void createOutline();
		void removeMeshAt(int i);
		void fillMeshIDs(GameObject* current);
		void deleteBuffers(int i);
		virtual void setType();
	};
}