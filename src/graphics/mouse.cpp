#include "mouse.h"
#include <iostream>

namespace gph = graphics;

void gph::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {}

void gph::moveCursorCallback(GLFWwindow* window, double xpos, double ypos) {
	if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_HIDDEN) {
		cameras[0]->rotate = true;
	}
}

void gph::mouseButtonCallback(GLFWwindow* window, int button, int state, int mods) {
	switch (button) {
	case GLFW_MOUSE_BUTTON_2:
		switch (state) {
		case GLFW_PRESS:
			cursorToMiddle(window);
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
			break;
		case GLFW_RELEASE:
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
		break;
	}
}

void gph::cursorToMiddle(GLFWwindow* window) {
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	glfwSetCursorPos(window, 0.5 * width, 0.5 * height);
}