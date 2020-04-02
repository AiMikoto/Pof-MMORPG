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
	engine::gpu->modelLoader->loadModel(modelPath);
	engine::gpu->modelLoader->loadModel(defaultModelPath);
	
	BOOST_LOG_TRIVIAL(trace) << glfwGetTime();
	for (int i = 0; i < 40; i++) {
		for (int j = 0; j < 40; j++) {
			engine::GameObject* model = new engine::GameObject();
			model->transform.position = glm::dvec3(2 * i - 40, 0, 2 * j - 40);
			//cube->transform.rotateTo(glm::dvec3(30, 20, 150));
			model->transform.scale = glm::dvec3(1, 1, 1);
			model->addComponent(new engine::MeshFilter());
			model->getComponent<engine::MeshFilter>()->modelPath = modelPath;
			model->getComponent<engine::MeshFilter>()->defaultModelPath = defaultModelPath;
			model->addComponent(new engine::MeshRenderer());
			
			scene->addGameObject(model);
		}
	}
	BOOST_LOG_TRIVIAL(trace) << glfwGetTime();
	glfwSwapInterval(0);
	BOOST_LOG_TRIVIAL(trace) << "Starting renderer";
	while (!engine::gpu->glContext->quit) {
		engine::gpu->update();
	}
	delete engine::gpu;
	return 0;
}
