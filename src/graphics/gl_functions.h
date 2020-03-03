#pragma once
#include "graphics_files.h"
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "objects.h"

namespace graphics {
	GLFWwindow * createGLFWContext(int width, int height, std::string name);
	void windowResizeCallback(GLFWwindow* window, int width, int height);
	void update(GLFWwindow* window, GameObject* mainScene, float lastTime, float check, int fps);
	void compileShader(std::string shader);
	void loadShaders(std::vector<std::string> shadersPaths);
	void updateCamera(GLFWwindow* window);
	void drawScene(GameObject* mainScene);
	void drawUI();
	void cleanup(GameObject* mainScene);

	struct ShaderLoader {
		GLuint vertexShaderID, fragmentShaderID;
		std::string vertexShaderPath, fragmentShaderPath;

		ShaderLoader(std::string vertexShaderPath, std::string fragmentShaderPath);
		~ShaderLoader();
		std::string readShaderFile(std::string path);
		GLuint loadShaders();
		void compileShader(std::string shader, GLuint shaderID);
		GLuint linkProgram();
	};
}