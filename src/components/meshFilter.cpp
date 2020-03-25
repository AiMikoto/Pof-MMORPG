#include "components/meshFilter.h"
#include "components/meshRenderer.h"
#include "graphics/gpu.h"

engine::MeshFilter::MeshFilter(std::string path) {
	this->path = path;
	this->meshesLoaded = false;
	setType();
}

engine::MeshFilter::MeshFilter(const MeshFilter& meshFilter) {
	this->path = meshFilter.path;
	for (auto id : meshFilter.subMeshesID) {
		this->subMeshesID.push_back(id);
	}
	this->meshesLoaded = true;
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
	if (meshesLoaded && !initialized) {
		MeshRenderer* meshRenderer = gameObject->getComponent<MeshRenderer>();
		if (meshRenderer != NULL) {
			meshRenderer->setup();
		}
		initialized = true;
	}
}

boost::property_tree::ptree engine::MeshFilter::serialize() {
	boost::property_tree::ptree node;
	node.add("path", path);
	return node;
}

engine::MeshFilter* engine::MeshFilter::deserialize(boost::property_tree::ptree node) {
	return this;
}