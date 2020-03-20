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
		uint meshID;
		uint materialID;

		MeshRenderer();
		~MeshRenderer();
		boost::property_tree::ptree serialize();
		void deserialize(boost::property_tree::ptree node);
		MeshRenderer* instantiate();
		void draw(Camera* camera, GLFWwindow* window);
		friend bool operator <(const MeshRenderer& r1, const MeshRenderer& r2) {
			return r1.materialID < r2.materialID;
		}
		virtual void setup();
		void meshLoaderRemoved();
	private:
		GLuint vertexArrayID, vertexBufferID, elementsBufferID, outlineIndicesBufferID;
		bool initialized = false;
		void glContextSetup();
		void bindBuffers();
		void createOutline();
		virtual void setType();
	};
}