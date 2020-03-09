#include "gl_functions.h"
#include "keyboard.h"
#include "utils.h"
#include "shader.h"
#include "transform.h"
#include "mouse.h"
#include "lib/log.h"

namespace gph = graphics;

GLFWwindow * gph::createGLFWContext(int width, int height, std::string name) {
	windowWidth = width;
	windowHeight = height;
  BOOST_LOG_TRIVIAL(trace) << "Attempting to initialise GLFW";
	if (!glfwInit())
	{
    BOOST_LOG_TRIVIAL(error) << "Failed to initialize GLFW";
		exit(-1);
	}
  BOOST_LOG_TRIVIAL(trace) << "Configuring GLFW";
	glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // We want OpenGL 4.0
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL 

  BOOST_LOG_TRIVIAL(trace) << "Open a window and create its OpenGL context";
	GLFWwindow* window = glfwCreateWindow(width, height, name.data(), NULL, NULL);
	if (window == NULL)
	{
    BOOST_LOG_TRIVIAL(error) << "Failed to open GLFW window!";
		exit(-1);
	}
	glfwMakeContextCurrent(window);
  BOOST_LOG_TRIVIAL(trace) << "Attempting to initialise GLAD";
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
    BOOST_LOG_TRIVIAL(error) << "Failed to initialize GLAD";
		exit(-1);
	}
	glClearColor(0, 0, 0, 1.0f);
	glViewport(0, 0, windowWidth, windowHeight);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	BOOST_LOG_TRIVIAL(trace) << "Setting callbacks for GLFW";

	glfwSetScrollCallback(window, scrollCallback);
	glfwSetCursorPosCallback(window, moveCursorCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetKeyCallback(window, keyboardCallback);
	glfwSetWindowSizeCallback(window, windowResizeCallback);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE);

	return window;
}

void gph::windowResizeCallback(GLFWwindow* window, int width, int height) {
	windowWidth = width;
	windowHeight = height;
	windowResized = true;
}

void gph::update(GLFWwindow* window, gph::GameObject* mainScene, double& lastTime, double& check, int fps) {
	double currentTime = glfwGetTime();
	deltaTime = currentTime - lastTime;
	lastTime = currentTime;

	if (windowResized) {
		windowResized = false;
	}

	updateCameras(window);
	drawScene(window, mainScene);

	glfwSwapBuffers(window);
	glfwPollEvents();
}


void gph::loadShaders(std::vector<std::string> shadersPath) {
	if (shadersPath.size() % 2) {
		std::cout << "The required shaders must be pairs of vertex and fragment shaders." << std::endl;
		std::cin.ignore();
		exit(-1);
	}
	for (size_t i = 0; i < shadersPath.size(); i += 2) {
		Shader* shader = new Shader(shadersPath[i], shadersPath[i + 1]);
		std::string name = split(shadersPath[i], '.')[0];
		std::vector<std::string> res = split(name, '/');
		shaderMap[res[res.size() - 1]] = shader;
	}
}

void gph::updateCameras(GLFWwindow* window) {
	for (auto camera : cameras) {
		for (int i = 0; i < totalCameraMovements; i++) {
			if (camera->moveBuffer[i])
				camera->moveCamera(i);
		}
		if (camera->rotate) {
			camera->rotateCamera(window);
		}
	}
}

void gph::setBackgroundColor(glm::vec4 color) {
	glClearColor(color.r, color.g, color.b, color.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void gph::setBackgroundColor(float r, float g, float b, float a) {
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void gph::setViewport(GLFWwindow* window, Camera* camera) {
	int x, y, width, height;
	glfwGetWindowSize(window, &width, &height);
	x = std::round(camera->viewport.startX * width);
	y = std::round(camera->viewport.startY * height);
	width = std::round(camera->viewport.endX * width);
	height = std::round(camera->viewport.endY * height);
	glScissor(x, y, width, height);
	glEnable(GL_SCISSOR_TEST);
	glViewport(x, y, width, height);
}

void gph::drawScene(GLFWwindow* window, gph::GameObject* mainScene) {
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	Shader* current = shaderMap[MODEL_SHADER];
	current->use();
	for (auto camera : cameras) {
		setViewport(window, camera);
		setBackgroundColor(colors::bgColor);
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 mvp = camera->projection(window) * camera->view() * model;
		current->setMat4("mvp", mvp);
		glBindVertexArray(vertexArrayID);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}	
}

void gph::drawUI() { }

void gph::cleanup(GameObject* mainScene) {
	BOOST_LOG_TRIVIAL(trace) << "Cleaning up";
	for (auto i : shaderMap) {
		delete i.second;
	}
	shaderMap.clear();
	for (auto camera : cameras) {
		delete camera;
	}
	cameras.clear();
	glDeleteVertexArrays(1, &vertexArrayID);
	glDeleteBuffers(1, &vertexBuffer);
	delete mainScene;
	glfwTerminate();
	BOOST_LOG_TRIVIAL(trace) << "cleanup successful";
}
