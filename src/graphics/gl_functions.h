#pragma once
#include "graphics_files.h"
#include <string>
#include <vector>
#include <string>
#include "objects.h"
#include "constants.h"
#include "variables.h"

namespace graphics {
	GLFWwindow * createGLFWContext(int width, int height, std::string name);
	void windowResizeCallback(GLFWwindow* window, int width, int height);
	void update(GLFWwindow* window, GameObject* mainScene, double lastTime, float check, int fps);
	void compileShader(std::string shader);
	void loadShaders(std::vector<std::string> shadersPaths);
	void updateCameras(GLFWwindow* window);
	void drawScene(GameObject* mainScene);
	void drawUI();
	void cleanup(GameObject* mainScene);	
}