#include "gl_functions.h"
#include "utils.h"
#include "camera.h"
#include "lib/log.h"
#include "model.h"

namespace gph = graphics;

int main() {
	log_init("graphics");
	BOOST_LOG_TRIVIAL(trace) << "Creating window";
	GLFWwindow* window = gph::createGLFWContext(gph::windowWidth, gph::windowHeight, gph::windowName);
	BOOST_LOG_TRIVIAL(trace) << "Window created";


	glGenVertexArrays(1, &gph::vertexArrayID);
	glGenBuffers(1, &gph::elementBufferID);
	glGenBuffers(1, &gph::vertexBufferID);
	glBindVertexArray(gph::vertexArrayID);
	
	glBindBuffer(GL_ARRAY_BUFFER, gph::vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(gph::vertices), &gph::vertices[0], GL_STATIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gph::elementBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(gph::indices), &gph::indices[0], GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, gph::vertexBufferID);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, gph::vertexBufferID);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, gph::vertexBufferID);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	glBindVertexArray(0);

	BOOST_LOG_TRIVIAL(trace) << "Creating scene";
	gph::GameObject* mainScene = new gph::GameObject();
	BOOST_LOG_TRIVIAL(trace) << "Creating camera";
	gph::cameras.push_back(new gph::Camera());
	mainScene->add_child(gph::cameras[0]->id);

	BOOST_LOG_TRIVIAL(trace) << "Creating shaders";
	std::vector<std::string> shaders = gph::charArrayToStringVector(gph::requiredShadersPath,
		(size_t)gph::requiredShadersPathLength);
	gph::loadShaders(shaders);
	gph::Model* model = new gph::Model("../src/graphics/objects/cube.obj", false);
	mainScene->add_child(model->id);
	//gph::loadTextures();

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
