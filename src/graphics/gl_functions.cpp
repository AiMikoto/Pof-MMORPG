#include "gl_functions.h"
#include "keyboard.h"
#include "utils.h"
#include "shader.h"
#include "camera.h"
#include "mouse.h"

namespace gph = graphics;



GLFWwindow * gph::createGLFWContext(int width, int height, std::string name) {
	windowWidth = width;
	windowHeight = height;
	if (!glfwInit())
	{
		std::cout << "Failed to initialize GLFW" << std::endl;
		std::cin.ignore();
		exit(-1);
	}
	glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // We want OpenGL 4.0
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL 

	// Open a window and create its OpenGL context
	GLFWwindow* window = glfwCreateWindow(width, height, name.data(), NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to open GLFW window!" << std::endl;
		glfwTerminate();
		std::cin.ignore();
		exit(-1);
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		std::cin.ignore();
		exit(-1);
	}
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glViewport(0, 0, windowWidth, windowHeight);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);
	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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

void gph::update(GLFWwindow* window, gph::GameObject* mainScene, double lastTime, float check, int fps) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	double currentTime = glfwGetTime();
	deltaTime = currentTime - lastTime;
	lastTime = currentTime;

	if (windowResized) {
		glViewport(0, 0, windowWidth, windowHeight);
		windowResized = false;
	}

	updateCameras(window);
	drawScene(mainScene);

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
		ShaderLoader shaderLoader = ShaderLoader(shadersPath[i], shadersPath[i + 1]);
		std::string name = split(shadersPath[i], '.')[0];
		std::vector<std::string> res = split(name, '/');
		programIDmap[res[res.size() - 1]] = shaderLoader.loadShaders();
	}
}

void gph::updateCameras(GLFWwindow* window) {
	for (auto camera : cameras) {
		if (camera->move) {
			for (int i = 0; i < totalCameraMovements; i++) {
				if (camera->moveBuffer[i])
					camera->moveCamera(i);
			}
		}
		if (camera->rotate) {
			camera->rotateCamera(window);
		}
	}
}

void gph::drawScene(gph::GameObject* mainScene) {
	glUseProgram(programIDmap[MODEL_SHADER]);
	glBindVertexArray(vertexArrayID);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

void gph::drawUI() { }

void gph::cleanup(GameObject* mainScene) {
	std::cout << "Cleaning up..." << std::endl;
	for (auto value: programIDmap) {
		glDeleteProgram(value.second);
	}
	programIDmap.clear();
	for (auto camera : cameras) {
		delete camera;
	}
	cameras.clear();
	glDeleteVertexArrays(1, &vertexArrayID);
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &elementBuffer);
	delete mainScene;
	glfwTerminate();
}
