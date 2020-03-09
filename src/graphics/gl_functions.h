#pragma once
#include "graphics_files.h"
#include <string>
#include <vector>
#include <string>
#include "objects.h"
#include "constants.h"
#include "variables.h"
#include "camera.h"

namespace graphics {
	GLFWwindow * createGLFWContext(int width, int height, std::string name);
	void windowResizeCallback(GLFWwindow* window, int width, int height);
	void update(GLFWwindow* window, GameObject* mainScene, double& lastTime, double& check, int fps);
	void setBackgroundColor(glm::vec4 color);
	void setBackgroundColor(float r, float g, float b, float a);
	void setViewport(GLFWwindow* window, Camera* camera);
	void compileShader(std::string shader);
	void loadShaders(std::vector<std::string> shadersPaths);
	void updateCameras(GLFWwindow* window);
	void drawScene(GLFWwindow* window, GameObject* mainScene);
	void drawUI();
	void cleanup(GameObject* mainScene);
	void loadTextures();
	uint loadTexture(std::string path);

	extern std::vector<uint> textures;
}