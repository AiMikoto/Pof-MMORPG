#pragma once
#include "graphics_files.h"
#include "objects.h"
#include "light.h"
#include "camera.h"
#include "constants.h"
#include "variables.h"
#include "shader.h"

namespace graphics {
	struct Vertex {
		glm::vec3 position, normal, tangent, bitangent;
		glm::vec2 textureCoordinates;
	};

	struct Texture {
		uint id;
		std::string type;
		std::string path;
	};

	class Mesh:GameObject {
	public:
		std::vector<Vertex> vertices;
		std::vector<Texture> textures;
		std::vector<uint> indices, outlineIndices;
		uchar drawMode;


		Mesh(std::vector<Vertex> vertices, std::vector<Texture> textures, std::vector<uint> indices);
		~Mesh();
		void draw(glm::mat4 &modelMatrix, glm::mat4 &viewMatrix, glm::mat4 &projectionMatrix, Camera* camera, std::vector<Light*> lights,
			bool drawOutline = false, glm::mat4 modelOutlineMatrix = glm::mat4(), bool drawWireFrame = false);
		void copy(Mesh* target);
	private:
		GLuint vertexArrayID, vertexBufferID, elementsBufferID, outlineIndicesBufferID;

		void setup();
		void bindBuffers();
		void createOutline();
	};
}