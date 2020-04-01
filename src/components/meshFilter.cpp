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
	node.add_child("Model", gpu->models[modelID]->serialize());
	return node;
}

engine::MeshFilter* engine::MeshFilter::deserialize(boost::property_tree::ptree node) {
	return this;
}

bool engine::MeshFilter::hasModel() {
	return gpu->models.count(modelID) == 1;
}