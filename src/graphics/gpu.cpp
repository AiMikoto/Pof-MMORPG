#include "graphics/gpu.h"
#include <algorithm>
#include "lib/log.h"
#include "core/time_values.h"
#include <boost/property_tree/json_parser.hpp>

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
	for (auto modelLayerMap : renderLayers) {
		for (auto matLayerMap : modelLayerMap.second) {
			delete matLayerMap.second;
		}
	}
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
	for (auto modelLayerMap : renderLayers) {
		for (auto matLayerMap : modelLayerMap.second) {
			RenderLayer* renderLayer = matLayerMap.second;
			renderLayer->draw(vp);
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
		for (uint i = 0; i < uint(renderer->materialIDs.size()); i++) {
			uint modelID = meshFilter->modelID;
			uint materialID = renderer->materialIDs[i];
			if (renderLayers[modelID].count(materialID) == 1) {
				RenderLayer* renderLayer = renderLayers[modelID][materialID];
				renderLayer->renderers.push_back(renderer);
			}
			else {
				RenderLayer* renderLayer = new RenderLayer(modelID, materialID, i);
				renderLayer->renderers.push_back(renderer);
				renderLayers[modelID][materialID] = renderLayer;
			}
			renderer->renderLayers.push_back(std::pair(modelID, materialID));
		}
	}
}

void engine::GPU::removeRenderer(MeshRenderer* renderer) {
	for (uint i = 0; i < uint(renderer->renderLayers.size()); i++) {
		std::pair<uint, uint> layerID = renderer->renderLayers[i];
		if (renderLayers.count(layerID.first) == 1) {
			if (renderLayers[layerID.first].count(layerID.second) == 1) { {
					RenderLayer* renderLayer = renderLayers[layerID.first][layerID.second];
					renderLayer->renderers.erase(std::remove(renderLayer->renderers.begin(),
						renderLayer->renderers.end(), renderer), renderLayer->renderers.end());
				}
			}
		}		
	}
}

void engine::GPU::removeModel(uint id) {
	if (renderLayers.count(id) == 1) {
		for (auto layer : renderLayers[id]) {
			delete layer.second;
		}
		renderLayers.erase(id);
	}
}