#include "graphics/gl_context.h"
#include "core/utils.h"
#include "components/camera.h"
#include "lib/log.h"
#include "graphics/model/mesh.h"
#include "scene/scene.h"
#include "graphics/gpu.h"
#include "graphics/model/model.h"
#include "components/meshRenderer.h"
#include "components/meshFilter.h"
#include "lib/nuklear.h"
#include "ui/ui.h"
#include "ui/console.h"

int main() {
	log_init("graphics");
	BOOST_LOG_TRIVIAL(trace) << "Initializing GPU context";
	engine::gpu = new engine::GPU();
	engine::gpu->initializeContext();
	BOOST_LOG_TRIVIAL(trace) << "GPU context initialized";

	BOOST_LOG_TRIVIAL(trace) << "Creating scene";
	engine::Scene* scene = new engine::Scene();
	engine::gpu->activeScenes.push_back(scene);

	std::string modelPath = "../src/graphics/assets/objects/kaguya.obj";
	std::string defaultModelPath = "../src/graphics/assets/objects/cube.obj";
	//engine::gpu->modelLoader->loadModel(modelPath);
	engine::gpu->modelLoader->loadModel(defaultModelPath);
	
	BOOST_LOG_TRIVIAL(trace) << glfwGetTime();
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			engine::GameObject* model = new engine::GameObject();
			model->transform.position = glm::dvec3(2 * i - 30, 0, 2 * j - 30);
			model->transform.rotateTo(glm::dvec3(6 * i, 6 * (i + j), 6 * j));
			model->transform.scale = glm::dvec3(1, 1, 1);
			model->addComponent(new engine::MeshFilter(modelPath));
			model->addComponent(new engine::MeshRenderer());
			
			scene->addGameObject(model);
		}
	}
	// scene->readFromFile("../src/graphics/assets/scenes/test.json");
	BOOST_LOG_TRIVIAL(trace) << scene->gameObjects.size();
	BOOST_LOG_TRIVIAL(trace) << glfwGetTime();
	glfwSwapInterval(0);
	BOOST_LOG_TRIVIAL(trace) << "Starting renderer";
	while (!engine::gpu->glContext->quit) {
		engine::gpu->update();
	}
	//scene->writeToFile("../src/graphics/assets/scenes/test.json");
	delete engine::gpu;
	return 0;
}
