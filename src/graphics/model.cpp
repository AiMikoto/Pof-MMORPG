#include "model.h"
#include "transform.h"
#include "light.h"
#define STB_IMAGE_IMPLEMENTATION
#include "lib/stb_image.h"
#include "lib/log.h"
#include "scene.h"
#include "utils.h"

namespace gph = graphics;

gph::Texture::Texture() {}

gph::Texture::Texture(std::string path) {
	this->path = path;
	load();
}

gph::Texture::Texture(std::string path, uint type) {
	this->path = path;
	this->type = type;
	load();
}

gph::Texture::~Texture() {
	glDeleteTextures(1, &id);
}

void gph::Texture::load() {
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, totalChannels;
	stbi_set_flip_vertically_on_load(true);
	uchar* data = stbi_load(path.c_str(), &width, &height, &totalChannels, 0);

	if (data) {
		GLenum format;
		switch (totalChannels) {
		case 1:
			format = GL_RED;
		case 3:
			format = GL_RGB;
		case 4:
			format = GL_RGBA;
		}
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		BOOST_LOG_TRIVIAL(trace) << "Failed to load texture at: " << path;
	}
	stbi_image_free(data);
	activeScene->textures[this->id] = this;
}

gph::Mesh::Mesh(std::vector<Vertex> vertices, std::vector<uint> textureIDs, std::vector<uint> indices) : gph::GameObject() {
	this->vertices = vertices;
	this->textureIDs = textureIDs;
	this->indices = indices;
	this->type = objectTypes::mesh;
	setup();
}

gph::Mesh::~Mesh() {
	glDeleteVertexArrays(1, &vertexArrayID);
	glDeleteBuffers(1, &vertexBufferID);
	glDeleteBuffers(1, &elementsBufferID);
	glDeleteBuffers(1, &outlineIndicesBufferID);
	vertices.clear();
	indices.clear();
	outlineIndices.clear();
}

void gph::Mesh::draw(Shader* shader, Camera* camera, GLFWwindow* window) {
	uint currentDiffuse = 1;
	uint currentSpecular = 1;
	uint currentAmbient = 1;
	uint currentEmmisive = 1;
	uint currentHeight = 1;
	uint currentNormal = 1;
	int index = 0;
	for (auto i: this->textureIDs) {
		glActiveTexture(GL_TEXTURE0 + i);
		std::string current;
		uint type = activeScene->textures[i]->type;
		switch (type) {
		case aiTextureType_DIFFUSE:
			current = "diffuseSampler" + std::to_string(currentDiffuse++);
			break;
		case aiTextureType_SPECULAR:
			break;
		case aiTextureType_AMBIENT:
			break;
		case aiTextureType_EMISSIVE:
			break;
		case aiTextureType_HEIGHT:
			break;
		case aiTextureType_NORMALS:
			break;
		}
		if (current != "") {
			shader->setInt(current, index);
			glBindTexture(GL_TEXTURE_2D, i);
		}
		index++;
	}

	glm::mat4 mvp = camera->projection(window) * camera->view() * transform.model();
	shader->setMat4("mvp", mvp);

	glBindVertexArray(vertexArrayID);
	glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glActiveTexture(GL_TEXTURE0);
}

void gph::Mesh::copy(Mesh* target) {}

void gph::Mesh::setup() {
//	bindBuffers();
	createOutline();
	computeScale();
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



gph::Model::Model(std::string path, bool gammaCorrection) {
	this->gammaCorrection = gammaCorrection;
	loadModel(path);
}

gph::Model::~Model() {}

void gph::Model::loadModel(std::string path) {
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

void gph::Model::processNode(aiNode* node, const aiScene* scene) {
	for (uint i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		addChild(processMesh(mesh, scene));
	}
	for (uint i = 0; i < node->mNumChildren; i++) {
		processNode(node->mChildren[i], scene);
	}
}

llong gph::Model::processMesh(aiMesh *mesh, const aiScene *scene) {
	std::vector<Vertex> vertices = loadMeshVertices(mesh, scene);
	std::vector<uint> indices = loadMeshIndices(mesh, scene);
	std::vector<uint> textureIDs = loadTextures(mesh, scene);
	Mesh* processedMesh = new Mesh(vertices, textureIDs, indices);
	activeScene->addMesh(processedMesh);
	return processedMesh->id;
}

std::vector<gph::Vertex> gph::Model::loadMeshVertices(aiMesh *mesh, const aiScene *scene) {
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

std::vector<uint> gph::Model::loadMeshIndices(aiMesh *mesh, const aiScene *scene) {
	std::vector<uint> indices;
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
	return indices;
}

std::vector<uint> gph::Model::loadTextures(aiMesh* mesh, const aiScene* scene) {
	std::vector<uint> textureIDs;
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	std::vector<uint> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE);
	textureIDs.insert(textureIDs.end(), diffuseMaps.begin(), diffuseMaps.end());
	std::vector<uint> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR);
	textureIDs.insert(textureIDs.end(), specularMaps.begin(), specularMaps.end());
	std::vector<uint> ambientMaps = loadMaterialTextures(material, aiTextureType_AMBIENT);
	textureIDs.insert(textureIDs.end(), ambientMaps.begin(), ambientMaps.end());
	std::vector<uint> emmisiveMaps = loadMaterialTextures(material, aiTextureType_EMISSIVE);
	textureIDs.insert(textureIDs.end(), emmisiveMaps.begin(), emmisiveMaps.end());
	std::vector<uint> heightMaps = loadMaterialTextures(material, aiTextureType_HEIGHT);
	textureIDs.insert(textureIDs.end(), heightMaps.begin(), heightMaps.end());
	std::vector<uint> normalMaps = loadMaterialTextures(material, aiTextureType_NORMALS);
	textureIDs.insert(textureIDs.end(), normalMaps.begin(), normalMaps.end());
	return textureIDs;
}

std::vector<uint> gph::Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type) {
	std::vector<uint> meshTextures;
	for (uint i = 0; i < mat->GetTextureCount(type); i++) {
		aiString path;
		mat->GetTexture(type, i, &path);
		bool skip = false;
		for (auto texture : activeScene->textures) {
			if (texture.second->path == std::string(path.C_Str())) {
				meshTextures.push_back(texture.second->id);
				skip = true;
				break;
			}
		}
		if (!skip) {
			Texture* texture = new Texture(directory + "/" + path.C_Str(), type); //textures are automatically placed in the map when instantiated
			meshTextures.push_back(texture->id);
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
	node.put("textureIDs", vectorToString(textureIDs, ' '));
	node.put("indices", vectorToString(indices, ' '));
	node.put("textures", vectorToString(outlineIndices, ' '));
	node.add_child("gameObject", GameObject::serialize());
	return node;
}

void gph::Mesh::deserialize(boost::property_tree::ptree node) {
	boost::property_tree::ptree verticesNode;

}
