#include "gl_functions.h"
#include "utils.h"
#include "camera.h"
#include "lib/log.h"
#include "mesh.h"
#include "scene.h"
#include "utils.h"
#include "gpu.h"

namespace gph = graphics;

int main() {
	log_init("graphics");
	BOOST_LOG_TRIVIAL(trace) << "Creating window";
	GLFWwindow* window = gph::createGLFWContext(gph::windowWidth, gph::windowHeight, gph::windowName);
	BOOST_LOG_TRIVIAL(trace) << "Window created";
	gph::gpu = new gph::GPU(window);

	BOOST_LOG_TRIVIAL(trace) << "Creating scene";
	gph::Scene* scene = new gph::Scene();
	gph::gpu->activeScenes.push_back(scene);

	BOOST_LOG_TRIVIAL(trace) << "Creating shaders";
	std::vector<std::string> shaders = gph::charArrayToStringVector(gph::requiredShadersPath,
		(size_t)gph::requiredShadersPathLength);
	gph::loadShaders(shaders);

	BOOST_LOG_TRIVIAL(trace) << "Creating camera";
	gph::GameObject* mainCamera = new gph::GameObject();
	mainCamera->name = "Main Camera";
	mainCamera->addComponent(new gph::Camera());
	mainCamera->transform.position = glm::dvec3(0, 0, 3);
	scene->gameObjects.push_back(mainCamera);

	BOOST_LOG_TRIVIAL(trace) << "Loading cube";
	gph::GameObject* cube = new gph::GameObject();
	cube->transform.position = glm::dvec3(5, 2, -20);
	cube->transform.rotateTo(glm::dvec3(30, 20, 150));
	cube->transform.scale = glm::dvec3(2, 2, 2);
	cube->addComponent(new gph::MeshLoader("../src/graphics/objects/cube.obj", false));
	cube->addComponent(new gph::MeshRenderer());
	scene->gameObjects.push_back(cube);

	double lastTime = glfwGetTime();
	double check = 0;
	int fps = 0;
	glfwSwapInterval(0);

	BOOST_LOG_TRIVIAL(trace) << "Starting renderer";
	while (!gph::quit) {
		gph::update(window, lastTime, check, fps);
	}
	gph::cleanup();
	return 0;
}
