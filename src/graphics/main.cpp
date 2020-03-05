#include "gl_functions.h"
#include "utils.h"
#include "camera.h"

namespace gph = graphics;

int main() {
	GLFWwindow* window = gph::createGLFWContext(gph::windowWidth, gph::windowHeight, gph::windowName);


	glGenVertexArrays(1, &gph::vertexArrayID);
	glGenBuffers(1, &gph::vertexBuffer);

	glBindVertexArray(gph::vertexArrayID);
	
	glBindBuffer(GL_ARRAY_BUFFER, gph::vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(gph::triangleVertices), &gph::triangleVertices, GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	gph::GameObject* mainScene = new gph::GameObject();
	gph::cameras.push_back(new gph::Camera()); //our main camera

	std::vector<std::string> shaders = gph::charArrayToStringVector(gph::requiredShadersPath,
		(size_t)gph::requiredShadersPathLength);
	gph::loadShaders(shaders);

	double lastTime = glfwGetTime();
	float check = 0;
	int fps = 0;
	glfwSwapInterval(0);
	std::cout << pi << std::endl;

	while (!gph::quit) {
		gph::update(window, mainScene, lastTime, check, fps);
	}
	gph::cleanup(mainScene);
	return 0;
}
