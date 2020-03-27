#include "meshRenderer.h"
#include "graphics/model/model.h"
#include "meshFilter.h"
#include "graphics/gpu.h"
#include "lib/log.h"

engine::MeshRenderer::MeshRenderer() {
	setType();
}

engine::MeshRenderer::~MeshRenderer() {
	cleanup();
}

boost::property_tree::ptree engine::MeshRenderer::serialize() {
	boost::property_tree::ptree node;
	return node;
}

engine::MeshRenderer* engine::MeshRenderer::deserialize(boost::property_tree::ptree node) {
	return this;
}

engine::MeshRenderer* engine::MeshRenderer::instantiate() {
	return this;
}

void engine::MeshRenderer::draw(Camera* camera, GLFWwindow* window, uint materialID) {
	glm::mat4 mvp = camera->projection(window) * camera->view() * gameObject->transform.model();
}

void engine::MeshRenderer::setup() {
	if (!initialized) {
		MeshFilter* meshFilter = gameObject->getComponent<MeshFilter>();
		if (meshFilter != NULL) {
			Model* model = gpu->models[meshFilter->modelID];
			this->modelID = meshFilter->modelID;
			for (auto mesh : model->meshes) {
				materialIDs.push_back(mesh->materialID);
			}
			gpu->addRenderer(this);
			initialized = true;
		}
	}
}

void engine::MeshRenderer::setType() {
	type = typeid(*this).name();
}

void engine::MeshRenderer::meshFilterRemoved() {
	cleanup();
	initialized = false;
}

void engine::MeshRenderer::modelRemoved() {
	materialIDs.clear();
	initialized = false;
}

void engine::MeshRenderer::cleanup() {
	if (initialized) {
		gpu->removeRenderer(this);
		materialIDs.clear();
	}
}