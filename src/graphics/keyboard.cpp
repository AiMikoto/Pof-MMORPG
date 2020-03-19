#include "keyboard.h"
#include "components/camera.h"
#include "common/scene.h"
#include "render/gpu.h"

void engine::keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	bool pressOrRepeat = (action == GLFW_PRESS || action == GLFW_REPEAT);
	switch (key)
	{
	case GLFW_KEY_ESCAPE:
		if (action == GLFW_PRESS)
			gpu->glContext->quit = true;
		break;
	case movementKeys::forward:
		gpu->editorCamera->moveBuffer[cam::cameraMovements::forward] = pressOrRepeat;
		break;
	case movementKeys::backwards:
		gpu->editorCamera->moveBuffer[cam::cameraMovements::backwards] = pressOrRepeat;
		break;
	case movementKeys::right:
		gpu->editorCamera->moveBuffer[cam::cameraMovements::right] = pressOrRepeat;
		break;
	case movementKeys::left:
		gpu->editorCamera->moveBuffer[cam::cameraMovements::left] = pressOrRepeat;
		break;
	}

}