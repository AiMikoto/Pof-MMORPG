#pragma once

#include "core/constants.h"
#include <vector>
#include <string>
#include "components/meshRenderer.h"
#include "include/glad.h"
#include "include/glfw3.h"

namespace engine {
	class RenderLayer {
	public:
		std::string materialPath, modelPath;
		uint submeshPos;
		std::vector<MeshRenderer*> renderers;

		RenderLayer(std::string modelPath, std::string materialPath, uint meshID);
		~RenderLayer();
		void draw(const glm::mat4& vp);
		void setup();
	private:
		GLuint vertexArrayID, vertexBufferID, elementsBufferID, outlineIndicesBufferID, instanceBufferID;
		void bindBuffers();
		void cleanup();
		std::vector<glm::mat4> getModelMatrices();
	};
}