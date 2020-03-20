#pragma once
#include "include/glad.h"
#include "include/glfw3.h"

namespace engine {
	void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
	void moveCursorCallback(GLFWwindow* window, double xpos, double ypos);
	void mouseButtonCallback(GLFWwindow* window, int button, int state, int mods);
	void cursorToMiddle(GLFWwindow* window);
}