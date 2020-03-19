#pragma once
#include "component.h"
#include "common/mesh.h"
#include <map>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "render/material.h"

namespace engine {
	class MeshLoader : public Component {
	public:
		bool gammaCorrection;
		uint meshID;
		std::string path;

		MeshLoader();
		MeshLoader(std::string path, bool gammaCorrection);
		~MeshLoader();
		boost::property_tree::ptree serialize();
		void deserialize(boost::property_tree::ptree node);
		MeshLoader* instantiate();
		virtual void setup();
	private:
		std::string directory;
		bool meshLoaded;
		void loadMesh(std::string path);
		void processNode(aiNode* node, const aiScene* scene);
		Mesh* processMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<Vertex> loadMeshVertices(aiMesh* mesh, const aiScene* scene);
		std::vector<uint> loadMeshIndices(aiMesh* mesh, const aiScene* scene);
		uint loadMaterial(aiMesh* mesh, const aiScene* scene);
		void loadMaterialTextures(aiMaterial* aiMat, aiTextureType type, Material* mat);
		virtual void setType();
	};
}