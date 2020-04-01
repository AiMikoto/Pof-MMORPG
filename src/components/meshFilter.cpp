#include "components/meshFilter.h"
#include "components/meshRenderer.h"
#include "graphics/gpu.h"

engine::MeshFilter::MeshFilter() {
	setType();
}

engine::MeshFilter::MeshFilter(const MeshFilter& meshFilter) {
	this->modelID = meshFilter.modelID;
	this->initialized = false;
}

engine::MeshFilter::MeshFilter(boost::property_tree::ptree) {
	
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
	return node;
}

bool engine::MeshFilter::hasModel() {
	return gpu->models.count(modelID) == 1;
}

double engine::MeshFilter::modelSize() {
	return size;
}

void engine::MeshFilter::computeModelSize() {
	std::vector<Mesh*> meshes = gpu->models[modelID]->meshes;
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