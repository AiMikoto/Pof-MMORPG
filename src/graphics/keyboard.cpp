#include "keyboard.h"
#include "camera.h"

namespace gph = graphics;

void gph::keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	switch (key)
	{
	case GLFW_KEY_ESCAPE:
		if (action == GLFW_PRESS)
			quit = true;
		break;
	case movementKeys::forward:
		cameras[0]->move[cameraMovements::forward] = (action == (GLFW_PRESS || GLFW_REPEAT));
		break;
	case movementKeys::backwards:
		cameras[0]->move[cameraMovements::backwards] = (action == (GLFW_PRESS || GLFW_REPEAT));
		break;
	case movementKeys::right:
		cameras[0]->move[cameraMovements::right] = (action == (GLFW_PRESS || GLFW_REPEAT));
		break;
	case movementKeys::left:
		cameras[0]->move[cameraMovements::left] = (action == (GLFW_PRESS || GLFW_REPEAT));
		break;
	}

}