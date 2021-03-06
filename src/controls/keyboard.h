#pragma once
#include "include/glad.h"
#include "include/glfw3.h"

namespace engine {
	enum movementKeys {
		forward = GLFW_KEY_W,
		backwards = GLFW_KEY_S,
		left = GLFW_KEY_A,
		right = GLFW_KEY_D
	};

	void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
}