#include "meshRenderer.h"
#include "graphics/model/model.h"
#include "meshFilter.h"
#include "graphics/gpu.h"
#include "lib/log.h"

engine::MeshRenderer::MeshRenderer() {
	setType();
}

engine::MeshRenderer::MeshRenderer(const MeshRenderer& renderer) {
	this->initialized = renderer.initialized;
	this->materialsPaths = renderer.materialsPaths;
	this->defaultMaterialPath = renderer.defaultMaterialPath;
	if(this->initialized && gpu != NULL)
		gpu->addRenderer(this);
	setType();
}

engine::MeshRenderer::MeshRenderer(boost::property_tree::ptree node) {
	for (auto mat : node.get_child("Materials")) {
		if(mat.first == "Material")
			materialsPaths.push_back(mat.second.get_value<std::string>());
		if (mat.first == "Default Material")
			defaultMaterialPath = mat.second.get_value<std::string>();
	}
	setType();
}

engine::MeshRenderer::~MeshRenderer() {
	cleanup();
}

boost::property_tree::ptree engine::MeshRenderer::serialize() {
	boost::property_tree::ptree node, matNode;
	matNode.add("Default Material", defaultMaterialPath);
	for (auto path : materialsPaths) {
		matNode.add("Material", path);
	}
	node.add_child("Materials", matNode);
	return node;
}

void engine::MeshRenderer::setup() {
	if (gpu != NULL) {
		if (!initialized) {
			MeshFilter* meshFilter = gameObject->getComponent<MeshFilter>();
			if (meshFilter != NULL) {
				Model* model = gpu->models[meshFilter->modelPath];
				if (materialsPaths.size() == 0) {
					for (auto mesh : model->meshes) {
						materialsPaths.push_back(mesh->materialPath);
					}
				}
				defaultMaterialPath = gpu->models[meshFilter->defaultModelPath]->meshes[0]->materialPath;
				gpu->addRenderer(this);
				initialized = true;
			}			
		}
	}
}

void engine::MeshRenderer::setType() {
	type = typeid(*this).name();
	name = "MeshRenderer";
}

void engine::MeshRenderer::meshFilterRemoved() {
	cleanup();
	initialized = false;
}

void engine::MeshRenderer::modelRemoved() {
	materialsPaths.clear();
	initialized = false;
}

void engine::MeshRenderer::cleanup() {
	if (initialized) {
		gpu->removeRenderer(this);
		materialsPaths.clear();
	}
}