#include "components/meshLoader.h"
#include "components/meshRenderer.h"
#include "graphics/gpu.h"
#include "lib/log.h"

engine::MeshLoader::MeshLoader() {
	setType();
}

engine::MeshLoader::MeshLoader(std::string path, bool gammaCorrection) {
	this->gammaCorrection = gammaCorrection;
	this->path = path;
	loadMesh(path);
	setType();
}

void engine::MeshLoader::setup() {
	if (!initialized) {
		MeshRenderer* meshRenderer = gameObject->getComponent<MeshRenderer>();
		if (meshRenderer != NULL) {
			meshRenderer->setup();
		}
		Component::setup();
	}
}

void engine::MeshLoader::setType() {
	type = typeidToClassName(typeid(this).name());
}

engine::MeshLoader::~MeshLoader() {
	if (initialized) {
		MeshRenderer* meshRenderer = gameObject->getComponent<MeshRenderer>();
		if (meshRenderer != NULL) {
			meshRenderer->meshLoaderRemoved();
		}
	}
}

void engine::MeshLoader::loadMesh(std::string path) {
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
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace |
			aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph);
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
			BOOST_LOG_TRIVIAL(trace) << "ERROR::ASSIMP:: " << importer.GetErrorString();
			return;
		}
		processNode(scene->mRootNode, scene);
		if (!meshLoaded)
			BOOST_LOG_TRIVIAL(trace) << "The loaded file contains no mesh";
	}
}

void engine::MeshLoader::processNode(aiNode* node, const aiScene* scene) {
	if (node->mNumMeshes > 1 || meshLoaded) {
		//TODO: assimp splits meshes if they have more than 1 material
		//either decide if all objects we're going to be dealing with will have 1 mesh 1 material, or we create a mesh merge algorithm
		//thinking on the latter, will use assimp's functions to merge them back, but I need to figure out how to store the materials
		BOOST_LOG_TRIVIAL(trace) << "Expected to load 1 mesh, found multiple";
	}
	else if (node->mNumMeshes == 1 && !meshLoaded) {
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

engine::Mesh* engine::MeshLoader::processMesh(aiMesh *mesh, const aiScene *scene) {
	std::vector<Vertex> vertices = loadMeshVertices(mesh, scene);
	std::vector<uint> indices = loadMeshIndices(mesh, scene);
	uint materialID = loadMaterial(mesh, scene);
	Mesh* processedMesh = new Mesh(vertices, materialID, indices);
	processedMesh->computeScale();
	return processedMesh;
}

std::vector<engine::Vertex> engine::MeshLoader::loadMeshVertices(aiMesh *mesh, const aiScene *scene) {
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

std::vector<uint> engine::MeshLoader::loadMeshIndices(aiMesh *mesh, const aiScene *scene) {
	std::vector<uint> indices;
	for (uint i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (uint j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
	return indices;
}

uint engine::MeshLoader::loadMaterial(aiMesh* mesh, const aiScene* scene) {
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
	mat->shaderID = shaderTypes::modelShader;
	gpu->materials[materialID] = mat;
	return materialID;
}

void engine::MeshLoader::loadMaterialTextures(aiMaterial* aiMat, aiTextureType type, Material* mat) {
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

boost::property_tree::ptree engine::MeshLoader::serialize() {
	boost::property_tree::ptree node;
	return node;
}

void engine::MeshLoader::deserialize(boost::property_tree::ptree node) {

}

engine::MeshLoader* engine::MeshLoader::instantiate() {
	return this;
}
