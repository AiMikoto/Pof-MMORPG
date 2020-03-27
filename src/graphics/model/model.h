#pragma once
#include "components/component.h"
#include "graphics/model/mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "graphics/model/material.h"
#include "components/camera.h"
#include "include/glad.h"
#include "include/glfw3.h"

namespace engine {
	class Model {
	public:
		std::vector<Mesh*> meshes;
		std::string path;
		std::string name;
		uint modelID;

		Model(std::string path);
		~Model();
		void draw(const std::vector<glm::mat4>& mvps, uint pos);
		void glContextSetup();
	private:
		bool initialized;
		std::vector<GLuint> vertexArrayID, vertexBufferID, elementsBufferID, outlineIndicesBufferID, instanceBufferID;
		void bindBuffers();
		void createOutline();
		void deleteBuffers(int i);
		void cleanup();
	};

	class ModelLoader {
	public:
		ModelLoader();
		~ModelLoader();
		void loadModel(std::string path, bool gammaCorrection = false);
	private:
		std::string directory;		
		void processNode(aiNode* node, const aiScene* scene, Model* model);
		Mesh* processMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<Vertex> loadMeshVertices(aiMesh* mesh, const aiScene* scene);
		std::vector<uint> loadMeshIndices(aiMesh* mesh, const aiScene* scene);
		uint loadMaterial(aiMesh* mesh, const aiScene* scene);
		void loadMaterialTextures(aiMaterial* aiMat, aiTextureType type, Material* mat);
	};
}