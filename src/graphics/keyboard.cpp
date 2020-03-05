#include "keyboard.h"
#include "camera.h"

namespace gph = graphics;

void gph::keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	bool pressOrRepeat = (action == GLFW_PRESS || action == GLFW_REPEAT);
	switch (key)
	{
	case GLFW_KEY_ESCAPE:
		if (action == GLFW_PRESS)
			quit = true;
		break;
	case movementKeys::forward:
		cameras[0]->moveBuffer[cam::cameraMovements::forward] = pressOrRepeat;
		cameras[0]->move = true;
		break;
	case movementKeys::backwards:
		cameras[0]->moveBuffer[cam::cameraMovements::backwards] = pressOrRepeat;
		cameras[0]->move = true;
		break;
	case movementKeys::right:
		cameras[0]->moveBuffer[cam::cameraMovements::right] = pressOrRepeat;
		cameras[0]->move = true;
		break;
	case movementKeys::left:
		cameras[0]->moveBuffer[cam::cameraMovements::left] = pressOrRepeat;
		cameras[0]->move = true;
		break;
	}

}