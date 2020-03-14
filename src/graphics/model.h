#pragma once
#include "graphics_files.h"
#include "objects.h"
#include "light.h"
#include "camera.h"
#include "constants.h"
#include "variables.h"
#include "shader.h"
#include <map>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <boost/property_tree/ptree.hpp>

namespace graphics {
	struct Vertex {
		glm::vec3 position, normal, tangent, bitangent;
		glm::vec2 textureCoordinates;
		boost::property_tree::ptree serialize();
		void deserialize(boost::property_tree::ptree node);
	};

	struct Texture {
		uint id;
		uint type;
		std::string path;

		Texture();
		Texture(std::string path);
		Texture(std::string path, uint type);
		~Texture();
		boost::property_tree::ptree serialize();
		void deserialize(boost::property_tree::ptree node);
	private:
		void load();
	};

	class Mesh : public GameObject {
	public:
		std::vector<Vertex> vertices;
		std::vector<uint> textureIDs;
		std::vector<uint> indices, outlineIndices;
		//represents the width, height and length of the inital loaded mesh
		glm::dvec3 meshScale;
		int drawMode;

		Mesh(std::vector<Vertex> vertices, std::vector<uint> textureIDs, std::vector<uint> indices);
		~Mesh();
		void draw(Shader* shader, Camera* camera, GLFWwindow* window);
		void copy(Mesh* target);
		boost::property_tree::ptree serialize();
		void deserialize(boost::property_tree::ptree node);
	private:
		GLuint vertexArrayID, vertexBufferID, elementsBufferID, outlineIndicesBufferID;
		void computeScale();
		void setup();
		void bindBuffers();
		void createOutline();
	};

	class Model : public GameObject {
	public:
		bool gammaCorrection;

		Model(std::string path, bool gammaCorrection);
		~Model();
	private:
		std::string directory;
		void loadModel(std::string path);
		void processNode(aiNode *node, const aiScene *scene);
		llong processMesh(aiMesh *mesh, const aiScene *scene);
		std::vector<Vertex> loadMeshVertices(aiMesh *mesh, const aiScene *scene);
		std::vector<uint> loadMeshIndices(aiMesh *mesh, const aiScene *scene);
		std::vector<uint> loadTextures(aiMesh *mesh, const aiScene *scene);
		std::vector<uint> loadMaterialTextures(aiMaterial *mat, aiTextureType type);
	};
}