#pragma once
#include "graphics_files.h"
#include "objects.h"
#include "light.h"
#include "camera.h"
#include "constants.h"
#include "variables.h"

namespace graphics {
	struct Vertex {
		glm::vec3 position, normal;
		glm::vec2 textureCoordinates;
	};

	struct Texture {
		uint id;
		std::string type;
	};

	struct Mesh:GameObject {
		std::vector<Vertex> vertices;
		std::vector<Texture> textures;
		std::vector<glm::uint> indices, outlineIndices;
		GLuint modelProgramID, outlineProgramID, wireframeProgramID;
		GLuint positionBuffer, textureBuffer, normalBuffer, elementBuffer, outlineBuffer;

		Mesh();
		~Mesh();
		void draw(glm::mat4 &modelMatrix, glm::mat4 &viewMatrix, glm::mat4 &projectionMatrix, Camera* camera, std::vector<Light*> lights,
			bool drawOutline = false, glm::mat4 modelOutlineMatrix = glm::mat4(), bool drawWireFrame = false);
		void drawModel(glm::mat4 &mvp, glm::mat4 &modelMatrix, Camera* camera, std::vector<Light*> lights);
		void drawWireFrame(glm::mat4 &mvp);
		void drawOutline(glm::mat4 &mvp);
		void createOutline();
		void bindBuffers();
		
		bool load(std::string path);
		void copy(Mesh* target);
	};
}