#pragma once
#include "graphics_files.h"
#include "objects.h"
#include "camera.h"
#include "component.h"
#include "constants.h"
#include "variables.h"
#include <map>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <boost/property_tree/ptree.hpp>
#include "material.h"

namespace graphics {
	struct Vertex {
		glm::vec3 position, normal, tangent, bitangent;
		glm::vec2 textureCoordinates;
		boost::property_tree::ptree serialize();
		void deserialize(boost::property_tree::ptree node);
	};	

	class Mesh : public Component {
	public:
		std::vector<Vertex> vertices;
		std::vector<uint> materialsIDs;
		std::vector<uint> indices, outlineIndices;
		glm::dvec3 meshScale;
		int drawMode;

		Mesh(std::vector<Vertex> vertices, std::vector<uint> materialsID, std::vector<uint> indices);
		~Mesh();
		void draw(Camera* camera, GLFWwindow* window);
		Mesh* instantiate(GameObject* gameObject);
		boost::property_tree::ptree serialize();
		void deserialize(boost::property_tree::ptree node);
		void computeScale();
		//only call on the rendering side
		void glContextSetup();
	private:
		GLuint vertexArrayID, vertexBufferID, elementsBufferID, outlineIndicesBufferID;
		bool initialized = false;
		void bindBuffers();
		void createOutline();
	};

	class MeshLoader {
	public:
		bool gammaCorrection;
		std::vector<Mesh*> loadedMeshes;

		MeshLoader(std::string path, bool gammaCorrection);
		~MeshLoader();
	private:
		std::string directory;
		void loadModel(std::string path);
		void processNode(aiNode *node, const aiScene *scene);
		Mesh* processMesh(aiMesh *mesh, const aiScene *scene);
		std::vector<Vertex> loadMeshVertices(aiMesh *mesh, const aiScene *scene);
		std::vector<uint> loadMeshIndices(aiMesh *mesh, const aiScene *scene);
		std::vector<uint> loadMaterials(aiMesh *mesh, const aiScene *scene);
		std::vector<uint> loadTextures(aiMesh *mesh, const aiScene *scene);
		std::vector<uint> loadMaterialTextures(aiMaterial *mat, aiTextureType type);
	};
}