#include "mesh.h"
#include "transform.h"
#include "light.h"
#include "lib/log.h"
#include "scene.h"
#include "utils.h"
#include "gpu.h"

namespace gph = graphics;

gph::Mesh::Mesh(std::vector<Vertex> vertices, std::vector<uint> materialsID, std::vector<uint> indices) {
	this->vertices = vertices;
	this->materialsIDs = materialsID;
	this->indices = indices;
	this->type = objectTypes::mesh;
}

gph::Mesh::~Mesh() {
	if (initialized) {
		glDeleteVertexArrays(1, &vertexArrayID);
		glDeleteBuffers(1, &vertexBufferID);
		glDeleteBuffers(1, &elementsBufferID);
		glDeleteBuffers(1, &outlineIndicesBufferID);
	}
	vertices.clear();
	indices.clear();
	outlineIndices.clear();
}

void gph::Mesh::draw(Camera* camera, GLFWwindow* window) {
	float alpha = 1.0f / float(materialsIDs.size());
	glm::mat4 mvp = camera->projection(window) * camera->view() * gameObject->transform.model();
	gpu->shaders[gpu->materials[0]->shaderID]->setMat4("mvp", mvp);
	for (auto i : materialsIDs) {
		gpu->materials[i]->shaderSetup(alpha);

		glBindVertexArray(vertexArrayID);
		glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glActiveTexture(GL_TEXTURE0);
	}
}

gph::Mesh* gph::Mesh::instantiate(GameObject* gameObject) {
	Mesh* mesh = new Mesh(this->vertices, this->materialsIDs, this->indices);
	mesh->gameObject = gameObject;
	return mesh;
}

void gph::Mesh::glContextSetup() {
	if (!initialized) {
		bindBuffers();
		createOutline();
		computeScale();
		initialized = true;
	}
}

void gph::Mesh::computeScale(){
	glm::dvec3 min = vertices[0].position;
	glm::dvec3 max = vertices[0].position;
	for (auto v : vertices) {
		min.x = glm::min(min.x, double(v.position.x));
		min.y = glm::min(min.y, double(v.position.y));
		min.z = glm::min(min.z, double(v.position.z));
		max.x = glm::max(max.x, double(v.position.x));
		max.y = glm::max(max.y, double(v.position.y));
		max.z = glm::max(max.z, double(v.position.z));
		if (v.position.x < min.x)
			min.x = v.position.x;
	}
	meshScale = max - min;
	BOOST_LOG_TRIVIAL(trace) << meshScale.x << ", " << meshScale.y << ", " << meshScale.z;
}

void gph::Mesh::bindBuffers() {
	glGenVertexArrays(1, &vertexArrayID);
	glGenBuffers(1, &vertexBufferID);
	glGenBuffers(1, &elementsBufferID);
	glGenBuffers(1, &outlineIndicesBufferID);

	glBindVertexArray(vertexArrayID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementsBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint), &indices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, textureCoordinates));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));

	glBindVertexArray(0);
}

void gph::Mesh::createOutline() {}

gph::MeshLoader::MeshLoader(std::string path, bool gammaCorrection) {
	this->gammaCorrection = gammaCorrection;
	loadModel(path);
}

gph::MeshLoader::~MeshLoader() {}

void gph::MeshLoader::loadModel(std::string path) {
	Assimp::Importer importer;
	BOOST_LOG_TRIVIAL(trace) << "Started loading model: " << path;
	directory = path.substr(0, path.find_last_of('/'));
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		BOOST_LOG_TRIVIAL(trace) << "ERROR::ASSIMP:: " << importer.GetErrorString();
		return;
	}
	processNode(scene->mRootNode, scene);
}

void gph::MeshLoader::processNode(aiNode* node, const aiScene* scene) {
	for (uint i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		loadedMeshes.push_back(processMesh(mesh, scene));
	}
	for (uint i = 0; i < node->mNumChildren; i++) {
		processNode(node->mChildren[i], scene);
	}
}

gph::Mesh* gph::MeshLoader::processMesh(aiMesh *mesh, const aiScene *scene) {
	std::vector<Vertex> vertices = loadMeshVertices(mesh, scene);
	std::vector<uint> indices = loadMeshIndices(mesh, scene);
	std::vector<uint> materialsIDs = loadTextures(mesh, scene);
	Mesh* processedMesh = new Mesh(vertices, materialsIDs, indices);
	processedMesh->computeScale();
	return processedMesh;
}

std::vector<gph::Vertex> gph::MeshLoader::loadMeshVertices(aiMesh *mesh, const aiScene *scene) {
	std::vector<Vertex> vertices;
	for (uint i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex;
		//convert to internal representation
		vertex.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		vertex.normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
		if (mesh->mTextureCoords[0])
			// this doesn't allow us to process models with more than a pair of texture coordinates/vertex
			vertex.textureCoordinates = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
		if (mesh->mTangents)
			vertex.tangent = glm::vec3(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);
		if(mesh->mBitangents)
			vertex.bitangent = glm::vec3(mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z);
		vertices.push_back(vertex);
	}
	return vertices;
}

std::vector<uint> gph::MeshLoader::loadMeshIndices(aiMesh *mesh, const aiScene *scene) {
	std::vector<uint> indices;
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
	return indices;
}

std::vector<uint> gph::MeshLoader::loadMaterials(aiMesh* mesh, const aiScene* scene) {
	std::vector<uint> materialsIDs;
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	std::vector<uint> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE);
	materialsIDs.insert(materialsIDs.end(), diffuseMaps.begin(), diffuseMaps.end());
	std::vector<uint> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR);
	materialsIDs.insert(materialsIDs.end(), specularMaps.begin(), specularMaps.end());
	std::vector<uint> ambientMaps = loadMaterialTextures(material, aiTextureType_AMBIENT);
	materialsIDs.insert(materialsIDs.end(), ambientMaps.begin(), ambientMaps.end());
	std::vector<uint> emmisiveMaps = loadMaterialTextures(material, aiTextureType_EMISSIVE);
	materialsIDs.insert(materialsIDs.end(), emmisiveMaps.begin(), emmisiveMaps.end());
	std::vector<uint> heightMaps = loadMaterialTextures(material, aiTextureType_HEIGHT);
	materialsIDs.insert(materialsIDs.end(), heightMaps.begin(), heightMaps.end());
	std::vector<uint> normalMaps = loadMaterialTextures(material, aiTextureType_NORMALS);
	materialsIDs.insert(materialsIDs.end(), normalMaps.begin(), normalMaps.end());
	return materialsIDs;
}

std::vector<uint> gph::MeshLoader::loadMaterialTextures(aiMaterial *mat, aiTextureType type) {
	std::vector<uint> meshTextures;
	for (uint i = 0; i < mat->GetTextureCount(type); i++) {
		aiString path;
		mat->GetTexture(type, i, &path);
		bool skip = false;
		for (auto texture : gpu->textures) {
			if (texture.second->path == std::string(path.C_Str())) {
				meshTextures.push_back(texture.second->id);
				skip = true;
				break;
			}
		}
		if (!skip) {
			Texture* texture = new Texture(directory + "/" + path.C_Str(), type); //textures are automatically placed in the map when instantiated
			meshTextures.push_back(texture->id);
			Material* mat = new Material(texture->id, gpu-);
		}
	}
	return meshTextures;
}

boost::property_tree::ptree gph::Vertex::serialize() {
	boost::property_tree::ptree node;
	node.add_child("position", dvec3serializer(position));
	node.add_child("normal", dvec3serializer(normal));
	node.add_child("textureCoordinates", dvec2serializer(textureCoordinates));
	node.add_child("tangent", dvec3serializer(tangent));
	node.add_child("bitangent", dvec3serializer(bitangent));
	return node;
}

void gph::Vertex::deserialize(boost::property_tree::ptree node) {

}

boost::property_tree::ptree gph::Texture::serialize() {
	boost::property_tree::ptree node;
	node.put("id", id);
	node.put("type", type);
	node.put("path", path);
	return node;
}

void gph::Texture::deserialize(boost::property_tree::ptree node) {
	id = node.get<uint>("id");
	type = node.get<uint>("type");
	path = node.get<std::string>("path");
}

boost::property_tree::ptree gph::Mesh::serialize() {
	boost::property_tree::ptree node, verticesNode;
	for (int i = 0; i < vertices.size(); i++) {
		verticesNode.add_child("v" + std::to_string(i), vertices[i].serialize());
	}
	node.add_child("vertices", verticesNode);
	node.put("materialIDs", vectorToString(materialsIDs, ' '));
	node.put("indices", vectorToString(indices, ' '));
	node.put("textures", vectorToString(outlineIndices, ' '));
	return node;
}

void gph::Mesh::deserialize(boost::property_tree::ptree node) {
	boost::property_tree::ptree verticesNode;

}