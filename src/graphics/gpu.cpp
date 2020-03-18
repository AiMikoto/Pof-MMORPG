#include "gpu.h"
#include "gl_functions.h"
#include <algorithm>

namespace gph = graphics;

gph::GPU* gph::gpu;

gph::GPU::GPU(GLFWwindow* window) {
	this->window = window;
}

gph::GPU::~GPU() {
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
	glfwTerminate();
}

void gph::GPU::draw() {
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	for (auto camera : cameras) {
		camera->setViewport(window);
		setBackgroundColor(colors::bgColor);
		for (auto mat : materials) {
			mat.second->contextSetup();
			int start = sortedRendererMat[mat.first].x;
			int end = sortedRendererMat[mat.first].y;
			for (int i = start; i <= end; i++) {
				renderers[i]->draw(camera, window);
			}
		}
		glActiveTexture(GL_TEXTURE0);
	}
}

void gph::GPU::sortRenderers() {
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

void gph::GPU::update() {
	for (auto s : activeScenes) {
		s->update();
	}
	gpu->draw();
}

void gph::GPU::addRenderer(MeshRenderer* renderer) {
	renderers.push_back(renderer);
	sortRenderers();
}

void gph::GPU::removeRenderer(MeshRenderer* renderer) {
	renderers.erase(std::remove(renderers.begin(), renderers.end(), renderer), renderers.end());
	sortRenderers();
}