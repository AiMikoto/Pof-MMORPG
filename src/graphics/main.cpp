#include "gl_functions.h"

namespace gph = graphics;

int main() {
	GLFWwindow* window = gph::createGLFWContext(gph::windowWidth, gph::windowHeight, gph::windowName);

	float lastTime = (float)glfwGetTime();
	float check = 0;
	int fps = 0;
	glfwSwapInterval(0);

	while (!gph::quit)
	{
		gph::update(window, lastTime, check, fps);
	}
	glfwTerminate();
	return 0;
}
