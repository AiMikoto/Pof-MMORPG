#pragma once
#include "graphics_files.h"
#include "constants.h"
#include "variables.h"
#include "camera.h"

namespace graphics {
	void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
	void moveCursorCallback(GLFWwindow* window, double xpos, double ypos);
	void mouseButtonCallback(GLFWwindow* window, int button, int state, int mods);
	void cursorToMiddle(GLFWwindow* window);
}