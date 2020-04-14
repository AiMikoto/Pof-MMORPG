#include "components/meshFilter.h"
#include "components/meshRenderer.h"
#include "graphics/gpu.h"
#include "core/utils.h"

engine::MeshFilter::MeshFilter(std::string path) {
	this->modelPath = path;
	this->defaultModelPath = path;
	this->aabbComputed = false;
	setType();
}

engine::MeshFilter::MeshFilter(const MeshFilter& meshFilter) {
	this->modelPath = meshFilter.modelPath;
	this->defaultModelPath = meshFilter.defaultModelPath;
	this->aabbPoints = meshFilter.aabbPoints;
	this->aabbComputed = meshFilter.aabbComputed;
	setType();
}

engine::MeshFilter::MeshFilter(boost::property_tree::ptree node) {
	modelPath = node.get<std::string>("model");
	defaultModelPath = node.get<std::string>("defaultModel");
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
	return node;
}

bool engine::MeshFilter::hasModel() {
	if (gpu == NULL) {
		return false;
	}
	return gpu->models.count(modelPath) == 1;
}

std::vector<glm::dvec3> engine::MeshFilter::computeAABB() {
	if (aabbComputed && gameObject->isStatic)
		return aabbPoints;
	glm::dvec3 size = gpu->models[modelPath]->modelSize();
	for (int j = 0; j < 2; j++) {
		for (int k = 0; k < 2; k++) {
			for (int l = 0; l < 2; l++) {
				glm::dvec4 point = {pow(-1, j) * size.x / 2, pow(-1, k) * size.y / 2, pow(-1, l) * size.z / 2 , 1.0};
				aabbPoints.push_back(gameObject->transform.model() * point);
			}
		}
	}
	aabbComputed = true;
	return aabbPoints;
}