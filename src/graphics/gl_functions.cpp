#include "gl_functions.h"
#include "keyboard_functions.h"
#include <iostream>

GLFWwindow * graphics::createGLFWContext(int width, int height, std::string name) {
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

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);
	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//glfwSetScrollCallback(window, scrollCallback);
	//glfwSetCursorPosCallback(window, moveCursorCallback);
	//glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetKeyCallback(window, keyboardCallback);
	glfwSetWindowSizeCallback(window, windowResizeCallback);

	return window;
}

void graphics::windowResizeCallback(GLFWwindow* window, int width, int height) {
	windowWidth = width;
	windowHeight = height;
	windowResized = true;
}

void graphics::Update(GLFWwindow* window, float lastTime, float check, int fps) {
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glfwSwapBuffers(window);
	glfwPollEvents();
}