#pragma once
#include "graphics_files.h"
#include <string>
#include <iostream>
#include <fstream>

namespace graphics {
	GLFWwindow * createGLFWContext(int width, int height, std::string name);
	void windowResizeCallback(GLFWwindow* window, int width, int height);
	void update(GLFWwindow* window, float lastTime, float check, int fps);
	GLuint loadShaders(std::string vertexFilePath, std::string fragmentFilePath);
	void loadRequiredShaders(std::vector<std::string> shadersPaths);
}