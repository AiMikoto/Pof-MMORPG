#include "mesh.h"
#include "transform.h"
#include "light.h"
#include "lib/log.h"
#include "scene.h"
#include "utils.h"
#include "gpu.h"
#include <typeinfo>

namespace gph = graphics;

gph::Mesh::Mesh(std::vector<Vertex> vertices, uint materialID, std::vector<uint> indices) {
	this->vertices = vertices;
	this->materialID = materialID;
	this->indices = indices;
}

gph::Mesh::~Mesh() {
	vertices.clear();
	indices.clear();
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

gph::MeshLoader::MeshLoader() {
	setType();
}

gph::MeshLoader::MeshLoader(std::string path, bool gammaCorrection) {
	this->gammaCorrection = gammaCorrection;
	this->path = path;
	loadMesh(path);
	setType();
}

void gph::MeshLoader::setup() {
	if (!initialized) {
		MeshRenderer* meshRenderer = gameObject->getComponent<MeshRenderer>();
		if (meshRenderer != NULL) {
			meshRenderer->setup();
		}
		Component::setup();
	}
}

void gph::MeshLoader::setType(){
	type = typeidToClassName(typeid(this).name());
}

gph::MeshLoader::~MeshLoader() {
	if (initialized) {
		MeshRenderer* meshRenderer = gameObject->getComponent<MeshRenderer>();
		if (meshRenderer != NULL) {
			meshRenderer->meshLoaderRemoved();
		}
	}
}

void gph::MeshLoader::loadMesh(std::string path) {
	bool preloaded = false;
	meshLoaded = false;
	for (auto mesh : gpu->meshes) {
		if (mesh.second->path == path) {
			BOOST_LOG_TRIVIAL(trace) << "Using preloaded mesh";
			meshID = mesh.first;
			preloaded = true;
			break;
		}			
	}
	if (!preloaded) {
		Assimp::Importer importer;
		BOOST_LOG_TRIVIAL(trace) << "Started loading mesh: " << path;
		directory = path.substr(0, path.find_last_of('/'));
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
			BOOST_LOG_TRIVIAL(trace) << "ERROR::ASSIMP:: " << importer.GetErrorString();
			return;
		}
		processNode(scene->mRootNode, scene);
		if(!meshLoaded)
			BOOST_LOG_TRIVIAL(trace) << "The loaded file contains no mesh";
	}
}

void gph::MeshLoader::processNode(aiNode* node, const aiScene* scene) {
	if (node->mNumMeshes > 1 || meshLoaded) {
		//TODO: assimp splits meshes if they have more than 1 material
		//either decide if all objects we're going to be dealing with will have 1 mesh 1 material, or we create a mesh merge algorithm
		BOOST_LOG_TRIVIAL(trace) << "Expected to load 1 mesh, found multiple";
	}
	else if(node->mNumMeshes == 1 && !meshLoaded) {
		Mesh* mesh = processMesh(scene->mMeshes[node->mMeshes[0]], scene);
		mesh->path = path;
		meshID = uint(gpu->meshes.size());
		gpu->meshes[meshID] = mesh;
		meshLoaded = true;
	}
	for (uint i = 0; i < node->mNumChildren; i++) {
		processNode(node->mChildren[i], scene);
	}
}

gph::Mesh* gph::MeshLoader::processMesh(aiMesh *mesh, const aiScene *scene) {
	std::vector<Vertex> vertices = loadMeshVertices(mesh, scene);
	std::vector<uint> indices = loadMeshIndices(mesh, scene);
	uint materialID = loadMaterial(mesh, scene);
	Mesh* processedMesh = new Mesh(vertices, materialID, indices);
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
			//TODO: add a way to obtain, store and pass more uvs to GLSL
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
	for (uint i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (uint j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
	return indices;
}

uint gph::MeshLoader::loadMaterial(aiMesh* mesh, const aiScene* scene) {
	aiMaterial* aiMat = scene->mMaterials[mesh->mMaterialIndex];
	Material* mat = new Material();
	aiMat->Get(AI_MATKEY_COLOR_DIFFUSE, mat->colorDiffuse);
	aiMat->Get(AI_MATKEY_COLOR_SPECULAR, mat->colorSpecular);
	aiMat->Get(AI_MATKEY_COLOR_AMBIENT, mat->colorAmbient);
	aiMat->Get(AI_MATKEY_COLOR_EMISSIVE, mat->colorEmissive);
	aiMat->Get(AI_MATKEY_COLOR_TRANSPARENT, mat->colorTransparent);
	aiMat->Get(AI_MATKEY_BLEND_FUNC, mat->blend);
	aiMat->Get(AI_MATKEY_SHADING_MODEL, mat->shading);
	aiMat->Get(AI_MATKEY_OPACITY, mat->opacity);
	aiMat->Get(AI_MATKEY_SHININESS, mat->shininess);
	aiMat->Get(AI_MATKEY_SHININESS_STRENGTH, mat->shininessStrength);
	for (int i = aiTextureType_DIFFUSE; i != aiTextureType_UNKNOWN; i++) {
		loadMaterialTextures(aiMat, aiTextureType(i), mat);
	}
	uint materialID = uint(gpu->materials.size());
	gpu->materials[materialID] = mat;
	return materialID;
}

void gph::MeshLoader::loadMaterialTextures(aiMaterial* aiMat, aiTextureType type, Material* mat) {
	for (uint i = 0; i < aiMat->GetTextureCount(type); i++) {
		aiString path;
		int textureOP;
		float textureStrength;
		aiMat->Get(AI_MATKEY_TEXOP(type, i), textureOP);
		aiMat->Get(AI_MATKEY_TEXBLEND(type, i), textureStrength);
		aiMat->GetTexture(type, i, &path);
		bool skip = false;
		for (auto texture : gpu->textures) {
			if (texture.second->path == std::string(path.C_Str())) {
				mat->texturesIDs.push_back(texture.second->id);
				
				
				mat->texturesOP.push_back(textureOP);
				skip = true;
				break;
			}
		}
		if (!skip) {
			Texture* texture = new Texture(directory + "/" + path.C_Str(), type); //textures are automatically placed in the map when instantiated
			mat->texturesIDs.push_back(texture->id);
		}
	}
}

boost::property_tree::ptree gph::MeshLoader::serialize() {
	boost::property_tree::ptree node;
	return node;
}

void gph::MeshLoader::deserialize(boost::property_tree::ptree node) {

}

gph::MeshLoader* gph::MeshLoader::instantiate() {
	return this;
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

boost::property_tree::ptree gph::Mesh::serialize() {
	boost::property_tree::ptree node, verticesNode;
	for (int i = 0; i < vertices.size(); i++) {
		verticesNode.add_child("v" + std::to_string(i), vertices[i].serialize());
	}
	node.add_child("vertices", verticesNode);
	node.put("materialID", materialID);
	node.put("indices", vectorToString(indices, ' '));
	return node;
}

void gph::Mesh::deserialize(boost::property_tree::ptree node) {
	boost::property_tree::ptree verticesNode;
}

gph::MeshRenderer::MeshRenderer() {
	setType();
}

gph::MeshRenderer::~MeshRenderer() {
	if (initialized) {
		gpu->removeRenderer(this);
	}
}

boost::property_tree::ptree gph::MeshRenderer::serialize() {
	boost::property_tree::ptree node;
	return node;
}

void gph::MeshRenderer::deserialize(boost::property_tree::ptree node) {}

gph::MeshRenderer* gph::MeshRenderer::instantiate() {
	return this;
}

void gph::MeshRenderer::glContextSetup() {
	if (!initialized) {
		bindBuffers();
		createOutline();
	}
}

void gph::MeshRenderer::bindBuffers() {
	Mesh* mesh = gpu->meshes[meshID];
	glGenVertexArrays(1, &vertexArrayID);
	glGenBuffers(1, &vertexBufferID);
	glGenBuffers(1, &elementsBufferID);
	glGenBuffers(1, &outlineIndicesBufferID);

	glBindVertexArray(vertexArrayID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, mesh->vertices.size() * sizeof(Vertex), &mesh->vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementsBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->indices.size() * sizeof(uint), &mesh->indices[0], GL_STATIC_DRAW);

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

void gph::MeshRenderer::createOutline() {}

void gph::MeshRenderer::draw(Camera* camera, GLFWwindow* window) {
	Mesh* mesh = gpu->meshes[meshID];
	Material* mat = gpu->materials[mesh->materialID];
	Shader* shader = gpu->shaders[mat->shaderID];
	glm::mat4 mvp = camera->projection(window) * camera->view() * gameObject->transform.model();
	shader->setMat4("mvp", mvp);
	glBindVertexArray(vertexArrayID);
	glDrawElements(GL_TRIANGLES, (GLsizei)mesh->indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void gph::MeshRenderer::setup() {
	if (!initialized) {
		MeshLoader* meshLoader = gameObject->getComponent<MeshLoader>();
		if (meshLoader != NULL) {
			meshID = meshLoader->meshID;
			materialID = gpu->meshes[meshID]->materialID;
			gpu->addRenderer(this);
			glContextSetup();
			initialized = true;
		}		
	}	
}

void gph::MeshRenderer::setType() {
	type = typeidToClassName(typeid(this).name());
}

void gph::MeshRenderer::meshLoaderRemoved() {
	initialized = false;
	gpu->removeRenderer(this);
}