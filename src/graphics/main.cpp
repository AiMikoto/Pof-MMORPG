#include "gl_functions.h"
#include "utils.h"

namespace gph = graphics;

int main() {
	GLFWwindow* window = gph::createGLFWContext(gph::windowWidth, gph::windowHeight, gph::windowName);

	GLuint vertexArrayID;
	glGenVertexArrays(1, &vertexArrayID);
	glBindVertexArray(vertexArrayID);

	gph::GameObject* mainScene = new gph::GameObject();

	std::vector<std::string> requiredShaders = gph::charArrayToStringVector(gph::requiredShadersPath,
		(size_t)gph::requiredShadersPathLength);
	gph::loadRequiredShaders(requiredShaders);

	float lastTime = (float)glfwGetTime();
	float check = 0;
	int fps = 0;
	glfwSwapInterval(0);

	while (!gph::quit) {
		gph::update(window, lastTime, check, fps);
	}
	gph::Cleanup(mainScene, vertexArrayID);
	glfwTerminate();
	return 0;
}
