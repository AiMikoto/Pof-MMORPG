#include "graphics/gl_context.h"
#include "core/utils.h"
#include "components/camera.h"
#include "lib/log.h"
#include "graphics/model/mesh.h"
#include "scene/scene.h"
#include "graphics/gpu.h"
#include "components/meshLoader.h"
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

	BOOST_LOG_TRIVIAL(trace) << "Loading cube";
	engine::GameObject* cube = new engine::GameObject();
	cube->transform.position = glm::dvec3(5, 2, -20);
	//cube->transform.rotateTo(glm::dvec3(30, 20, 150));
	cube->transform.scale = glm::dvec3(2, 2, 2);
	cube->addComponent(new engine::MeshLoader("../src/graphics/assets/objects/kaguya.obj", false));
	cube->addComponent(new engine::MeshRenderer());
	scene->addGameObject(cube);

	glfwSwapInterval(0);

	BOOST_LOG_TRIVIAL(trace) << "Starting renderer";
	while (!engine::gpu->glContext->quit) {
		engine::gpu->update();
	}
	delete engine::gpu;
	return 0;
}
