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
		int drawMode;

		Mesh(std::vector<Vertex> vertices, std::vector<Texture> textures, std::vector<uint> indices);
		~Mesh();
		void draw(Shader* shader);
		void copy(Mesh* target);
	private:
		GLuint vertexArrayID, vertexBufferID, elementsBufferID, outlineIndicesBufferID;

		void setup();
		void bindBuffers();
		void createOutline();
	};
}