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

	class Mesh {
	public:
		std::vector<Vertex> vertices;
		uint materialID;
		std::vector<uint> indices;
		glm::dvec3 meshScale;
		std::string path;

		Mesh(std::vector<Vertex> vertices, uint materialID, std::vector<uint> indices);
		~Mesh();
		boost::property_tree::ptree serialize();
		void deserialize(boost::property_tree::ptree node);
		void computeScale();	
	};

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

	class MeshRenderer : public Component {
	public:
		uint meshID;
		uint materialID;

		MeshRenderer();
		~MeshRenderer();
		boost::property_tree::ptree serialize();
		void deserialize(boost::property_tree::ptree node);
		MeshRenderer* instantiate();
		void draw(Camera* camera, GLFWwindow* window);
		friend bool operator <(const MeshRenderer& r1, const MeshRenderer& r2) {
			return r1.materialID < r2.materialID;
		}
		virtual void setup();
		
		void meshLoaderRemoved();
	private:
		GLuint vertexArrayID, vertexBufferID, elementsBufferID, outlineIndicesBufferID;
		bool initialized = false;
		void glContextSetup();
		void bindBuffers();
		void createOutline();
		virtual void setType();
	};
}