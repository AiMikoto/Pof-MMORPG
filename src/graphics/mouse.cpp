#include "mouse.h"
#include "common/scene.h"
#include <iostream>
#include "render/gpu.h"

void engine::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {}

void engine::moveCursorCallback(GLFWwindow* window, double xpos, double ypos) {
	if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_HIDDEN) {
		gpu->editorCamera->rotate = true;
	}
}

void engine::mouseButtonCallback(GLFWwindow* window, int button, int state, int mods) {
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

void engine::cursorToMiddle(GLFWwindow* window) {
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	glfwSetCursorPos(window, 0.5 * width, 0.5 * height);
}