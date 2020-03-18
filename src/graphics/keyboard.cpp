#include "keyboard.h"
#include "camera.h"
#include "scene.h"
#include "gpu.h"

namespace gph = graphics;

void gph::keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	bool pressOrRepeat = (action == GLFW_PRESS || action == GLFW_REPEAT);
	Camera* mainCamera = gpu->cameras[0];
	switch (key)
	{
	case GLFW_KEY_ESCAPE:
		if (action == GLFW_PRESS)
			quit = true;
		break;
	case movementKeys::forward:
		mainCamera->moveBuffer[cam::cameraMovements::forward] = pressOrRepeat;
		break;
	case movementKeys::backwards:
		mainCamera->moveBuffer[cam::cameraMovements::backwards] = pressOrRepeat;
		break;
	case movementKeys::right:
		mainCamera->moveBuffer[cam::cameraMovements::right] = pressOrRepeat;
		break;
	case movementKeys::left:
		mainCamera->moveBuffer[cam::cameraMovements::left] = pressOrRepeat;
		break;
	}

}