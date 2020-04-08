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
	delete modelLoader;
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
	modelLoader = new ModelLoader();
}

#define MAX_VERTEX_BUFFER 512 * 1024
#define MAX_ELEMENT_BUFFER 128 * 1024

void engine::GPU::initializeGUI() {
	BOOST_LOG_TRIVIAL(trace) << "Attempting to initialise nuklear context";
	ctx = nk_glfw3_init(glContext -> window, NK_GLFW3_INSTALL_CALLBACKS, MAX_VERTEX_BUFFER, MAX_ELEMENT_BUFFER);
	BOOST_LOG_TRIVIAL(trace) << "Loading font";
	{
		struct nk_font_atlas *atlas;
		nk_glfw3_font_stash_begin(&atlas);
		nk_glfw3_font_stash_end();
	}
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
	nk_glfw3_new_frame();
	enum {EASY, HARD};
	static int op = EASY;
	static float value = 0.6f;
	static int i =  20;
	if (nk_begin(ctx, "Demo", nk_rect(50, 50, 230, 250), NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE | NK_WINDOW_MINIMIZABLE | NK_WINDOW_TITLE)) {
		BOOST_LOG_TRIVIAL(trace) << "Box created";
		nk_layout_row_static(ctx, 30, 80, 1);
		if (nk_button_label(ctx, "button")) {
			BOOST_LOG_TRIVIAL(trace) << "button pressed";
		}
	}
	nk_end(ctx);
	nk_glfw3_render(NK_ANTI_ALIASING_ON);
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
		for (uint i = 0; i < uint(renderer->materialsPaths.size()); i++) {
			std::string modelPath = meshFilter->modelPath;
			std::string materialPath = renderer->materialsPaths[i];
			if (renderLayers[modelPath].count(materialPath) == 1) {
				RenderLayer* renderLayer = renderLayers[modelPath][materialPath];
				renderLayer->renderers.push_back(renderer);
			}
			else {
				RenderLayer* renderLayer = new RenderLayer(modelPath, materialPath, i);
				renderLayer->renderers.push_back(renderer);
				renderLayers[modelPath][materialPath] = renderLayer;
			}
			renderer->renderLayers.push_back(std::pair(modelPath, materialPath));
		}
	}
}

void engine::GPU::removeRenderer(MeshRenderer* renderer) {
	for (uint i = 0; i < uint(renderer->renderLayers.size()); i++) {
		std::pair<std::string, std::string> layerID = renderer->renderLayers[i];
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

void engine::GPU::removeModel(std::string path) {
	if (renderLayers.count(path) == 1) {
		for (auto layer : renderLayers[path]) {
			delete layer.second;
		}
		renderLayers.erase(path);
	}
}
