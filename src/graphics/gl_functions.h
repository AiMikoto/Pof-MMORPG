#pragma once
#include "graphics_files.h"
#include <string>

namespace graphics {
	GLFWwindow * createGLFWContext(int width, int height, std::string name);
	void windowResizeCallback(GLFWwindow* window, int width, int height);
	void Update(GLFWwindow* window, float lastTime, float check, int fps);
}