#include "gpu.h"
#include <algorithm>
#include "lib/log.h"
#include "common/time.h"

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
	renderers.clear();
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
	cameras.erase(cameras.begin(), cameras.end());
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
		int start = sortedRendererMat[mat.first].x;
		int end = sortedRendererMat[mat.first].y;
		for (int i = start; i <= end; i++) {
			renderers[i]->draw(editorCamera, glContext->window);
		}
	}
	glActiveTexture(GL_TEXTURE0);
}

void engine::GPU::drawUI() {
	glDisable(GL_DEPTH_TEST);
}

void engine::GPU::sortRenderers() {
	std::sort(renderers.begin(), renderers.end());
	uint lastMatID = renderers[0]->materialID;
	glm::ivec2 matToRenderer = glm::ivec2(0, 0);
	for (int i = 1; i < renderers.size(); i++) {
		if (renderers[i]->materialID != lastMatID) {
			matToRenderer.y = i - 1;
			sortedRendererMat[lastMatID] = matToRenderer;
			matToRenderer.x = i;
		}
		lastMatID = renderers[i]->materialID;
	}
	matToRenderer.y = int(renderers.size() - 1);
	sortedRendererMat[lastMatID] = matToRenderer;
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
	renderers.push_back(renderer);
	sortRenderers();
}

void engine::GPU::removeRenderer(MeshRenderer* renderer) {
	renderers.erase(std::remove(renderers.begin(), renderers.end(), renderer), renderers.end());
	sortRenderers();
}