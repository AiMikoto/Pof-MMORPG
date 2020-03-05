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
	gph::cameras.push_back(new gph::Camera(gph::CameraViewport(0, 0, gph::windowWidth / 2, gph::windowHeight / 2))); //our main camera
	gph::cameras.push_back(new gph::Camera(gph::CameraViewport(gph::windowWidth / 2, gph::windowHeight / 2,
		gph::windowWidth, gph::windowHeight))); //testing 2nd camera

	std::vector<std::string> shaders = gph::charArrayToStringVector(gph::requiredShadersPath,
		(size_t)gph::requiredShadersPathLength);
	gph::loadShaders(shaders);

	double lastTime = glfwGetTime();
	double check = 0;
	int fps = 0;
	glfwSwapInterval(0);

	while (!gph::quit) {
		gph::update(window, mainScene, lastTime, check, fps);
	}
	gph::cleanup(mainScene);
	return 0;
}
