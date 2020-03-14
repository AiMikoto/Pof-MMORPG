#include "gl_functions.h"
#include "utils.h"
#include "camera.h"
#include "lib/log.h"
#include "model.h"
#include "scene.h"
#include "utils.h"

namespace gph = graphics;

int main() {
	log_init("graphics");
	BOOST_LOG_TRIVIAL(trace) << "Creating window";
	GLFWwindow* window = gph::createGLFWContext(gph::windowWidth, gph::windowHeight, gph::windowName);
	BOOST_LOG_TRIVIAL(trace) << "Window created";

	BOOST_LOG_TRIVIAL(trace) << "Creating scene";
	gph::activeScene = new gph::Scene();
	BOOST_LOG_TRIVIAL(trace) << gph::activeScene->id;
	BOOST_LOG_TRIVIAL(trace) << "Creating camera";
	gph::activeScene->addCamera(new gph::Camera(), true);

	BOOST_LOG_TRIVIAL(trace) << "Creating shaders";
	std::vector<std::string> shaders = gph::charArrayToStringVector(gph::requiredShadersPath,
		(size_t)gph::requiredShadersPathLength);
	gph::loadShaders(shaders);
	BOOST_LOG_TRIVIAL(trace) << "Loading model";
	gph::Model* model = new gph::Model("../src/graphics/objects/cube.obj", false);
	model->transform.position = glm::dvec3(5, 2, -20);
	model->transform.rotateTo(glm::radians(45.0), glm::dvec3(1, 1, 1));
	model->transform.scale = glm::dvec3(2, 2, 2);
	gph::activeScene->addChild(model->id);	

	gph::activeScene->sceneToJSON("../src/graphics/scenes/test.json");

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
