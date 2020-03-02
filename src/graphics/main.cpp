#include "gl_functions.h"
#include "utils.h"

namespace gph = graphics;

int main() {
	GLFWwindow* window = gph::createGLFWContext(gph::windowWidth, gph::windowHeight, gph::windowName);
	std::vector<std::string> c = gph::charArrayToStringVector(gph::requiredShadersPath, (size_t)gph::requiredShadersPathLength);
	for (auto s : c) {
		std::cout << s << std::endl;
	}

	float lastTime = (float)glfwGetTime();
	float check = 0;
	int fps = 0;
	glfwSwapInterval(0);

	while (!gph::quit) {
		gph::update(window, lastTime, check, fps);
	}
	glfwTerminate();
	return 0;
}
