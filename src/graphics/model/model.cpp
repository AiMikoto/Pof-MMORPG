#include "graphics/model/model.h"
#include "components/meshRenderer.h"
#include "components/meshFilter.h"
#include "graphics/gpu.h"
#include "lib/log.h"
#include "core/exceptions.h"
#include <typeinfo>
#include "core/utils.h"

engine::Model::Model(std::string path) {
	this->path = path;
	this->name = path.substr(path.find_last_of('/') + 1, path.size() - 1);
	initialized = false;
	BOOST_LOG_TRIVIAL(trace) << this->name;
}

engine::Model::~Model() {
	for (auto mesh : meshes) {
		delete mesh;
	}
	meshes.clear();
	gpu->removeModel(modelID);
	cleanup();
}

void engine::Model::glContextSetup() {
	bindBuffers();
	createOutline();
}

void engine::Model::bindBuffers() {
	for (int i = 0; i < meshes.size(); i++) {
		vertexArrayID.push_back(0);
		vertexBufferID.push_back(0);
		elementsBufferID.push_back(0);
		outlineIndicesBufferID.push_back(0);
		glGenVertexArrays(1, &vertexArrayID[i]);
		glGenBuffers(1, &vertexBufferID[i]);
		glGenBuffers(1, &elementsBufferID[i]);
		glGenBuffers(1, &outlineIndicesBufferID[i]);

		glBindVertexArray(vertexArrayID[i]);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID[i]);
		glBufferData(GL_ARRAY_BUFFER, meshes[i]->vertices.size() * sizeof(Vertex), &meshes[i]->vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementsBufferID[i]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshes[i]->indices.size() * sizeof(uint), &meshes[i]->indices[0], GL_STATIC_DRAW);

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
}

void engine::Model::createOutline() {}

void engine::Model::draw(const glm::mat4& mvp, uint materialID) {
	int pos = 0;
	for (int i = 0; i < meshes.size(); i++) {
		if (materialID == meshes[i]->materialID) {
			pos = i;
			break;
		}
	}
	Mesh* mesh = meshes[pos];
	Material* mat = gpu->materials[materialID];
	Shader* shader = gpu->shaders[mat->shaderID];
	shader->setMat4("mvp", mvp);
	glBindVertexArray(vertexArrayID[pos]);
	glDrawElements(GL_TRIANGLES, (GLsizei)mesh->indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void engine::Model::deleteBuffers(int i) {
	glDeleteVertexArrays(1, &vertexArrayID[i]);
	glDeleteBuffers(1, &vertexBufferID[i]);
	glDeleteBuffers(1, &elementsBufferID[i]);
	glDeleteBuffers(1, &outlineIndicesBufferID[i]);
}

void engine::Model::cleanup() {
	if (initialized) {
		for (int i = 0; i < meshes.size(); i++) {
			deleteBuffers(i);
		}
		vertexArrayID.clear();
		vertexBufferID.clear();
		elementsBufferID.clear();
		outlineIndicesBufferID.clear();
	}
}

engine::ModelLoader::ModelLoader() {}

engine::ModelLoader::~ModelLoader() {}

void engine::ModelLoader::loadModel(std::string path, bool gammaCorrection) {
	if (gpu->modelsPaths.count(path)) {
		BOOST_LOG_TRIVIAL(trace) << "Model already loaded: " << path;
		return;
	}
	BOOST_LOG_TRIVIAL(trace) << "Started loading model: " << path;
	Assimp::Importer importer;
	directory = path.substr(0, path.find_last_of('/'));
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace |
		aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		throw assimp_error(importer.GetErrorString());
	Model* model = new Model(path);
	processNode(scene->mRootNode, scene, model);
	uint modelID = getFirstAvailableMapIndex(gpu->models);
	model->glContextSetup();
	gpu->models[modelID] = model;
	gpu->modelsPaths[path] = modelID;
	model->modelID = modelID;
}

void engine::ModelLoader::processNode(aiNode* node, const aiScene* scene, Model* model) {
	for (uint i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		Mesh* processedMesh = processMesh(mesh, scene);
		processedMesh->name = mesh->mName.C_Str();
		model->meshes.push_back(processedMesh);
	}
	for (uint i = 0; i < node->mNumChildren; i++) {
		processNode(node->mChildren[i], scene, model);
	}
}

engine::Mesh* engine::ModelLoader::processMesh(aiMesh *mesh, const aiScene *scene) {
	std::vector<Vertex> vertices = loadMeshVertices(mesh, scene);
	std::vector<uint> indices = loadMeshIndices(mesh, scene);
	uint materialID = loadMaterial(mesh, scene);
	Mesh* processedMesh = new Mesh(vertices, materialID, indices);
	processedMesh->computeScale();
	return processedMesh;
}

std::vector<engine::Vertex> engine::ModelLoader::loadMeshVertices(aiMesh *mesh, const aiScene *scene) {
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
		if (mesh->mBitangents)
			vertex.bitangent = glm::vec3(mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z);
		vertices.push_back(vertex);
	}
	return vertices;
}

std::vector<uint> engine::ModelLoader::loadMeshIndices(aiMesh *mesh, const aiScene *scene) {
	std::vector<uint> indices;
	for (uint i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (uint j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
	return indices;
}

uint engine::ModelLoader::loadMaterial(aiMesh* mesh, const aiScene* scene) {
	aiMaterial* aiMat = scene->mMaterials[mesh->mMaterialIndex];
	Material* mat = new Material();

	aiColor4D aiColor;
	aiMat->Get(AI_MATKEY_COLOR_DIFFUSE, aiColor);
	mat->colorDiffuse = glm::vec4(aiColor.r, aiColor.g, aiColor.b, aiColor.a);

	aiMat->Get(AI_MATKEY_COLOR_SPECULAR, aiColor);
	mat->colorSpecular = glm::vec4(aiColor.r, aiColor.g, aiColor.b, aiColor.a);

	aiMat->Get(AI_MATKEY_COLOR_AMBIENT, aiColor);
	mat->colorAmbient = glm::vec4(aiColor.r, aiColor.g, aiColor.b, aiColor.a);

	aiMat->Get(AI_MATKEY_COLOR_EMISSIVE, aiColor);
	mat->colorEmissive = glm::vec4(aiColor.r, aiColor.g, aiColor.b, aiColor.a);

	aiMat->Get(AI_MATKEY_COLOR_TRANSPARENT, aiColor);
	mat->colorTransparent = glm::vec4(aiColor.r, aiColor.g, aiColor.b, aiColor.a);

	aiMat->Get(AI_MATKEY_BLEND_FUNC, mat->blend);
	aiMat->Get(AI_MATKEY_SHADING_MODEL, mat->shading);
	aiMat->Get(AI_MATKEY_OPACITY, mat->opacity);
	aiMat->Get(AI_MATKEY_SHININESS, mat->shininess);
	aiMat->Get(AI_MATKEY_SHININESS_STRENGTH, mat->shininessStrength);
	for (int i = aiTextureType_DIFFUSE; i != aiTextureType_UNKNOWN; i++) {
		loadMaterialTextures(aiMat, aiTextureType(i), mat);
	}
	uint materialID = getFirstAvailableMapIndex(gpu->materials);
	mat->shaderID = shaderTypes::modelShader;
	gpu->materials[materialID] = mat;
	return materialID;
}

void engine::ModelLoader::loadMaterialTextures(aiMaterial* aiMat, aiTextureType type, Material* mat) {
	for (uint i = 0; i < aiMat->GetTextureCount(type); i++) {
		aiString path;
		int textureOP;
		float textureStrength;
		aiMat->Get(AI_MATKEY_TEXOP(type, i), textureOP);
		aiMat->Get(AI_MATKEY_TEXBLEND(type, i), textureStrength);
		aiMat->GetTexture(type, i, &path);
		bool skip = false;
		for (auto texture : gpu->textures) {
			if (texture.second->path == directory + "/" + std::string(path.C_Str())) {
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