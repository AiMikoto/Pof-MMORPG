#include "gl_functions.h"
#include "utils.h"

namespace gph = graphics;

int main() {
	GLFWwindow* window = gph::createGLFWContext(gph::windowWidth, gph::windowHeight, gph::windowName);

	glGenVertexArrays(1, &gph::vertexArrayID);
	glBindVertexArray(gph::vertexArrayID);
	
	glGenBuffers(1, &gph::vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, gph::vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(gph::triangleVertices), &gph::triangleVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	

	gph::GameObject* mainScene = new gph::GameObject();

	std::vector<std::string> shaders = gph::charArrayToStringVector(gph::requiredShadersPath,
		(size_t)gph::requiredShadersPathLength);
	gph::loadShaders(shaders);

	float lastTime = (float)glfwGetTime();
	float check = 0;
	int fps = 0;
	glfwSwapInterval(0);

	while (!gph::quit) {
		gph::update(window, mainScene, lastTime, check, fps);
	}
	gph::Cleanup(mainScene);
	glfwTerminate();
	return 0;
}
