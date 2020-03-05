#include "mouse.h"
#include <iostream>

namespace gph = graphics;

void gph::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {}

void gph::moveCursorCallback(GLFWwindow* window, double xpos, double ypos) {
	int inputMode = glfwGetInputMode(window, GLFW_CURSOR);
	if (inputMode == GLFW_CURSOR_DISABLED) {
		cameras[0]->rotate = true;
		//std::cout << xpos << std::endl;
	}
}

void gph::mouseButtonCallback(GLFWwindow* window, int button, int state, int mods) {
	switch (button) {
	case GLFW_MOUSE_BUTTON_1:
		switch (state) {
		case GLFW_PRESS:
			int inputMode = glfwGetInputMode(window, GLFW_CURSOR);
			if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			else {
				cursorToMiddle(window);
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			}
			break;
		}
		break;
	}
}

void gph::cursorToMiddle(GLFWwindow* window) {
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	glfwSetCursorPos(window, 0.5 * width, 0.5 * height);
}