#pragma once
#include "component.h"
#include "common/objects.h"
#include "camera.h"
#include "render/graphics_files.h"

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