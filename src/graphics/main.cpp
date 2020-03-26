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

	engine::gpu->meshLoader->loadModel("../src/graphics/assets/objects/kaguya.obj");
	uint modelID = engine::gpu->models.begin()->first;
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 20; j++) {
			engine::GameObject* model = new engine::GameObject();
			model->transform.position = glm::dvec3(3 * i - 30, 0, 3 * j - 30);
			//cube->transform.rotateTo(glm::dvec3(30, 20, 150));
			model->transform.scale = glm::dvec3(2, 2, 2);
			model->addComponent(new engine::MeshFilter(modelID)); 
			model->addComponent(new engine::MeshRenderer());
			scene->addGameObject(model);
		}
	}

	glfwSwapInterval(0);
	bool deleted1 = false;
	bool deleted2 = false;
	bool added1 = false;
	bool added2 = false;

	BOOST_LOG_TRIVIAL(trace) << "Starting renderer";
	while (!engine::gpu->glContext->quit) {
		engine::gpu->update();
	}
	delete engine::gpu;
	return 0;
}
