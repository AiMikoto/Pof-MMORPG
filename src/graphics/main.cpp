#include "gl_functions.h"
#include "utils.h"
#include "camera.h"
#include "lib/log.h"

namespace gph = graphics;

int main() 
{
  log_init("graphics");
	BOOST_LOG_TRIVIAL(trace) << "Creating window";
	GLFWwindow* window = gph::createGLFWContext(gph::windowWidth, gph::windowHeight, gph::windowName);
	BOOST_LOG_TRIVIAL(trace) << "Window created";


	glGenVertexArrays(1, &gph::vertexArrayID);
	glGenBuffers(1, &gph::vertexBuffer);

	glBindVertexArray(gph::vertexArrayID);
	
	glBindBuffer(GL_ARRAY_BUFFER, gph::vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(gph::triangleVertices), &gph::triangleVertices, GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	BOOST_LOG_TRIVIAL(trace) << "Creating scene";
	gph::GameObject* mainScene = new gph::GameObject();
	BOOST_LOG_TRIVIAL(trace) << "Creating camera";
	gph::cameras.push_back(new gph::Camera());

	BOOST_LOG_TRIVIAL(trace) << "Creating shaders";
	std::vector<std::string> shaders = gph::charArrayToStringVector(gph::requiredShadersPath,
		(size_t)gph::requiredShadersPathLength);
	gph::loadShaders(shaders);

	double lastTime = glfwGetTime();
	double check = 0;
	int fps = 0;
	glfwSwapInterval(0);

	BOOST_LOG_TRIVIAL(trace) << "Starting renderer";
	while (!gph::quit) {
		gph::update(window, mainScene, lastTime, check, fps);
	}
	gph::cleanup(mainScene);
	return 0;
}
