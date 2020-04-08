#include "components/meshFilter.h"
#include "components/meshRenderer.h"
#include "graphics/gpu.h"

engine::MeshFilter::MeshFilter(std::string path) {
	this->modelPath = path;
	this->defaultModelPath = path;
	this->size = 0;
	setType();
}

engine::MeshFilter::MeshFilter(const MeshFilter& meshFilter) {
	this->modelPath = meshFilter.modelPath;
	this->defaultModelPath = meshFilter.defaultModelPath;
	this->size = meshFilter.size;
	setType();
}

engine::MeshFilter::MeshFilter(boost::property_tree::ptree node) {
	modelPath = node.get<std::string>("model");
	defaultModelPath = node.get<std::string>("defaultModel");
	size = node.get<double>("modelSize");
	setType();
}

engine::MeshFilter::~MeshFilter() {
	if (initialized) {
		MeshRenderer* meshRenderer = gameObject->getComponent<MeshRenderer>();
		if (meshRenderer != NULL) {
			meshRenderer->meshFilterRemoved();
		}
	}
}

void engine::MeshFilter::setType() {
	type = typeid(*this).name();
	name = "MeshFilter";
}

// implements lazy loading
void engine::MeshFilter::assertModel()
{
	if (hasModel()) {
		return;
	}
	// time to load
	if (gpu != NULL) {
		gpu->modelLoader->loadModel(modelPath);
	}
}

void engine::MeshFilter::setup() {
	assertModel();
	if (gpu != NULL && hasModel() && !initialized) {
		MeshRenderer* meshRenderer = gameObject->getComponent<MeshRenderer>();
		if (meshRenderer != NULL) {
			meshRenderer->setup();
		}
		initialized = true;
	}
}

boost::property_tree::ptree engine::MeshFilter::serialize() {
	boost::property_tree::ptree node;
	node.add("model", modelPath);
	node.add("defaultModel", defaultModelPath);
	node.add("modelSize", size);
	return node;
}

bool engine::MeshFilter::hasModel() {
	if (gpu == NULL) {
		return false;
	}
	return gpu->models.count(modelPath) == 1;
}

double engine::MeshFilter::modelSize() {
	return size;
}

void engine::MeshFilter::computeModelSize() {
	std::vector<Mesh*> meshes = gpu->models[modelPath]->meshes;
	glm::vec3 center = { 0,0,0 };
	size = glm::distance(center, meshes[0]->vertices[0].position);
	
	for (auto mesh : meshes) {
		for (auto v : mesh->vertices) {
			double dist = glm::distance(center, v.position);
			if (dist > size)
				size = dist;
		}
	}
}
