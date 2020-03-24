#include "graphics/gpu.h"
#include <algorithm>
#include "lib/log.h"
#include "core/time_values.h"

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
	meshes.clear();
	cameras.clear();
	renderLayers.clear();
	renderLayersMap.clear();
	for (auto s : activeScenes) {
		delete s;
	}
	activeScenes.clear();
	delete glContext;
	delete editorCamera->gameObject;
	BOOST_LOG_TRIVIAL(trace) << "cleanup successful";
}

void engine::GPU::initializeContext() {
	BOOST_LOG_TRIVIAL(trace) << "Creating openGL context";
	this->glContext = new GLContext(1280, 720, "Pof Engine");
	BOOST_LOG_TRIVIAL(trace) << "Creating editor camera";
	GameObject* editorCamera = new GameObject();
	editorCamera->addComponent(new Camera());
	editorCamera->transform.position = glm::dvec3(0, 0, 3);
	this->editorCamera = editorCamera->getComponent<Camera>();
	cameras.erase(cameras.begin());
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
	for (auto mat : materials) {
		mat.second->contextSetup();
		for (auto r : renderLayers[renderLayersMap[mat.first]]) {
			r->draw(editorCamera, glContext->window, mat.first);
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
	determineRenderLayers(renderer);
}

void engine::GPU::removeRenderer(MeshRenderer* renderer) {
	for (auto i : renderer->materialIDs) {
		renderLayers[renderLayersMap[i]].erase(std::remove(renderLayers[renderLayersMap[i]].begin(),
			renderLayers[renderLayersMap[i]].end(),
			renderer),
			renderLayers[renderLayersMap[i]].end());
	}	
}

void engine::GPU::determineRenderLayers(MeshRenderer* renderer) {
	for (auto i : renderer->materialIDs) {
		renderLayers[renderLayersMap[i]].push_back(renderer);
	}
}