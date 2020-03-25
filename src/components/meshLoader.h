#pragma once
#include "components/component.h"
#include "graphics/model/mesh.h"
#include <map>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "graphics/model/material.h"

namespace engine {
	class MeshLoader {
	public:
		MeshLoader();
		~MeshLoader();
		void loadMesh(GameObject* gameObject, std::string path, bool gammaCorrection);
	private:
		std::string directory;		
		void processNode(aiNode* node, const aiScene* scene, GameObject* gameObject);
		Mesh* processMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<Vertex> loadMeshVertices(aiMesh* mesh, const aiScene* scene);
		std::vector<uint> loadMeshIndices(aiMesh* mesh, const aiScene* scene);
		uint loadMaterial(aiMesh* mesh, const aiScene* scene);
		void loadMaterialTextures(aiMaterial* aiMat, aiTextureType type, Material* mat);
	};
}