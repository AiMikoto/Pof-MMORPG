#include "components/meshFilter.h"
#include "components/meshRenderer.h"
#include "graphics/gpu.h"

engine::MeshFilter::MeshFilter() {
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
	//for now load the model on scene initialization
	if (gpu != NULL)
		gpu->modelLoader->loadModel(modelPath);
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

void engine::MeshFilter::setup() {
	if (hasModel() && !initialized) {
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