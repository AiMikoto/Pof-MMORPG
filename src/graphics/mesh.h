#pragma once
#include "graphics_files.h"
#include "objects.h"
#include "light.h"

namespace graphics {
	struct Mesh:GameObject {
		std::vector<glm::vec3> vertices, normals;
		std::vector<glm::vec2> uvs;
		std::vector<glm::uint> indices, outlineIndices;
		GLuint texture, textureID, modelProgramID, outlineProgramID, wireframeProgramID;
		GLuint positionBuffer, colorBuffer, textureBuffer, normalBuffer, elementBuffer, outlineBuffer;

		Mesh();
		~Mesh();
		void draw(glm::mat4 &modelMatrix, glm::mat4 &viewMatrix, glm::mat4 &projectionMatrix, Camera* camera, std::vector<Light*> lights,
			bool drawOutline = false, glm::mat4 modelOutlineMatrix = glm::mat4(), bool drawWireFrame = false);
		void drawModel(glm::mat4 &mvp, glm::mat4 &modelMatrix, Camera* camera, std::vector<Light*> lights);
		void drawWireFrame(glm::mat4 &mvp);
		void drawOutline(glm::mat4 &mvp);
		void createOutline();
		void bindBuffers();
		void setUniform(const GLchar* uniformName, GLuint programID, float set);
		void setUniform(const GLchar* uniformName, GLuint programID, glm::vec3 set);
		void setUniform(const GLchar* uniformName, GLuint programID, glm::vec4 set);
		void setUniform(const GLchar* uniformName, GLuint programID, glm::mat4 set);
		bool load(std::string path);
		void copy(Mesh* target);
	};
}