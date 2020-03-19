#include "gl_context.h"
#include "keyboard.h"
#include "utils.h"
#include "shader.h"
#include "mouse.h"
#include "lib/log.h"
#include "gpu.h"

engine::GLContext::GLContext(int width, int height, std::string name) {
	this->width = width;
	this->height = height;
	this->name = name;
	createGLFWContext();
}

engine::GLContext::~GLContext() {
	glfwTerminate();
}

void engine::GLContext::createGLFWContext() {
	initGLFW();
	configureGLFW();
	openWindow();
	initGLAD();
	setGLFWcallbacks();
	setGLvalues();
	std::vector<std::string> shaders = charArrayToStringVector(requiredShadersPath,
		(size_t)requiredShadersPathLength);
	loadShaders(shaders);
}

void engine::GLContext::initGLFW() {
	BOOST_LOG_TRIVIAL(trace) << "Attempting to initialise GLFW";
	if (!glfwInit()) {
		BOOST_LOG_TRIVIAL(error) << "Failed to initialize GLFW";
		exit(-1);
	}
}

void engine::GLContext::configureGLFW() {
	BOOST_LOG_TRIVIAL(trace) << "Configuring GLFW";
	glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // We want OpenGL 4.0
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL
}

void engine::GLContext::openWindow() {
	BOOST_LOG_TRIVIAL(trace) << "Open a window and create its OpenGL context";
	window = glfwCreateWindow(width, height, name.data(), NULL, NULL);
	if (window == NULL) {
		BOOST_LOG_TRIVIAL(error) << "Failed to open GLFW window!";
		exit(-1);
	}
	glfwMakeContextCurrent(window);
}

void engine::GLContext::initGLAD() {
	BOOST_LOG_TRIVIAL(trace) << "Attempting to initialise GLAD";
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		BOOST_LOG_TRIVIAL(error) << "Failed to initialize GLAD";
		exit(-1);
	}
}

void engine::GLContext::setGLFWcallbacks() {
	BOOST_LOG_TRIVIAL(trace) << "Setting callbacks for GLFW";
	glfwSetScrollCallback(window, scrollCallback);
	glfwSetCursorPosCallback(window, moveCursorCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetKeyCallback(window, keyboardCallback);
	glfwSetWindowSizeCallback(window, windowResizeCallback);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE);
}

void engine::GLContext::setGLvalues() {
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void engine::windowResizeCallback(GLFWwindow* window, int width, int height) {
	gpu->glContext->width = width;
	gpu->glContext->height = height;
	gpu->glContext->windowResized = true;
}

void engine::GLContext::update() {
	glfwSwapBuffers(window);
	glfwPollEvents();
}

void engine::GLContext::loadShaders(std::vector<std::string> shadersPath) {
	BOOST_LOG_TRIVIAL(trace) << "Creating shaders";
	if (shadersPath.size() % 2) {
		BOOST_LOG_TRIVIAL(error) << "The required shaders must be pairs of vertex and fragment shaders.";
		exit(-1);
	}
	for (int i = 0; i < shadersPath.size(); i += 2) {
		Shader* shader = new Shader(shadersPath[i], shadersPath[i + 1]);
		gpu->shaders[i/2] = shader;
	}
}

void engine::GLContext::setBackgroundColor(glm::vec4 color) {
	glClearColor(color.r, color.g, color.b, color.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void engine::GLContext::setBackgroundColor(float r, float g, float b, float a) {
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
