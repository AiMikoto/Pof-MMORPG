#include "graphics/gpu.h"
#include <algorithm>
#include "lib/log.h"
#include "core/time_values.h"
#include <boost/property_tree/json_parser.hpp>

engine::GPU* engine::gpu;

engine::GPU::GPU() {}

engine::GPU::~GPU() {
	BOOST_LOG_TRIVIAL(trace) << "Cleaning up";
	for (auto t : textures) {
		delete t.second;
	}
	textures.clear();
	for (auto m : materials) {
		delete m.second;
	}
	materials.clear();
	for (auto s : shaders) {
		delete s.second;
	}
	shaders.clear();
	cameras.clear();
	renderLayers.clear();
	for (auto s : activeScenes) {
		delete s;
	}
	for (auto m : models) {
		delete m.second;
	}
	models.clear();
	activeScenes.clear();
	delete glContext;
	delete editorCamera->gameObject;
	delete meshLoader;
	BOOST_LOG_TRIVIAL(trace) << "cleanup successful";
}

void engine::GPU::initializeContext() {
	BOOST_LOG_TRIVIAL(trace) << "Creating openGL context";
	glContext = new GLContext(1280, 720, "Pof Engine");
	BOOST_LOG_TRIVIAL(trace) << "Creating editor camera";
	GameObject* editorCamera = new GameObject();
	editorCamera->addComponent(new Camera());
	editorCamera->transform.position = glm::dvec3(0, 5, 20);
	this->editorCamera = editorCamera->getComponent<Camera>();
	cameras.erase(cameras.begin());
	meshLoader = new ModelLoader();
}

void engine::GPU::draw() {
	drawScene();
	drawUI();
}

void engine::GPU::drawScene() {
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	editorCamera->setViewport(glContext->window);
	glContext->setBackgroundColor(colors::bgColor);
	glm::mat4 vp = editorCamera->projection(glContext->window) * editorCamera->view();
	for (auto m : models) {
		for (uint i = 0; i < (uint)m.second->meshes.size(); i++) {
			materials[m.second->meshes[i]->materialID]->contextSetup();
			shaders[materials[m.second->meshes[i]->materialID]->shaderID]->setMat4("vp", vp);
			int j = 0;
			while (j < renderLayers[m.first].size()) {
				std::vector<glm::mat4> objectsToRender;
				for (uint k = uint(objectsToRender.size()); k < 10000 && j < renderLayers[m.first].size(); k = uint(objectsToRender.size())) {
					objectsToRender.push_back(renderLayers[m.first][j]->gameObject->transform.model());
					j++;
				}
				m.second->draw(objectsToRender, i);
			}
		}
	}
	glActiveTexture(GL_TEXTURE0);
}

void engine::GPU::drawUI() {
	glDisable(GL_DEPTH_TEST);
}

void engine::GPU::update() {
	Time::updateTimes();
	for (auto s : activeScenes) {
		s->update();
	}
	gpu->draw();
	gpu->editorCamera->update();
	gpu->glContext->update();
}

void engine::GPU::addRenderer(MeshRenderer* renderer) {
	MeshFilter* meshFilter = renderer->gameObject->getComponent<MeshFilter>();
	if (meshFilter != NULL) {
		renderLayers[meshFilter->modelID].push_back(renderer);
		renderer->renderLayer = std::pair<uint, uint>(meshFilter->modelID, 0);
	}
}

void engine::GPU::removeRenderer(MeshRenderer* renderer) {
	uint id = renderer->renderLayer.first;
	renderLayers[id].erase(std::remove(renderLayers[id].begin(), renderLayers[id].end(), renderer), renderLayers[id].end());
}

void engine::GPU::removeModel(uint id) {
	if (renderLayers.count(id) == 1) {
		for (auto renderer : renderLayers[id]) {
			renderer->modelRemoved();
		}
		renderLayers.erase(id);
	}
}

boost::property_tree::ptree engine::GPU::serialize() {
	boost::property_tree::ptree node, texNode, matNode, modelsNode, scenesNode;
	node.add_child("Textures", texNode);
	node.add_child("Materials", matNode);
	node.add_child("Models", modelsNode);
	node.add_child("Scenes", scenesNode);
	for (auto t : textures) {
		texNode.add_child("Texture", t.second->serialize());
	}
	for (auto m : materials) {
		matNode.add_child("Material", m.second->serialize());
	}
	for (auto m : models) {
		modelsNode.add_child("Model", m.second->serialize());
	}
	for (auto s : activeScenes) {
		scenesNode.add_child("Scene", s->serialize());
	}
}

engine::GPU* engine::GPU::deserialize(boost::property_tree::ptree node) {


}

void engine::GPU::saveToJSON(std::string path) {
	boost::property_tree::ptree root = serialize();
	boost::property_tree::write_json(path, root);
}

void engine::GPU::loadFromJSON(std::string path) {

}