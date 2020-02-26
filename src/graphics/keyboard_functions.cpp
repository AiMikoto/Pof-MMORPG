#include "keyboard_functions.h"

void graphics::keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	switch (key)
	{
	case GLFW_KEY_ESCAPE:
		if (action == GLFW_PRESS)
			quit = true;
		break;
	}
}