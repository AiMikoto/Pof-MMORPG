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
#include <boost/property_tree/ptree.hpp>
#include "glm/gtc/type_ptr.hpp"

namespace engine {
	class Model {
	public:
		std::vector<Mesh*> meshes;
		std::string path;
		std::string name;
		bool loaded;
		

		Model(std::string path);
		~Model();
		glm::dvec3 modelSize();
		glm::dvec3 computeSize();
	private:
		bool sizeComputed;
		glm::dvec3 size;
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
		std::string loadMaterial(aiMesh* mesh, const aiScene* scene);
		void loadMaterialTextures(aiMaterial* aiMat, aiTextureType type, Material* mat);
	};
}