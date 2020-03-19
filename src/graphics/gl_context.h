#pragma once
#include "include/glad.h"
#include "include/glfw3.h"
#include "glm/gtc/type_ptr.hpp"
#include <string>
#include <vector>

namespace engine {
	void windowResizeCallback(GLFWwindow* window, int width, int height);

	class GLContext {
	public:
		GLFWwindow* window;
		std::string name;
		int width, height;
		bool quit, windowResized;
		double fps;

		GLContext(int width, int height, std::string name);
		~GLContext();
		void update();
		void setBackgroundColor(glm::vec4 color);
		void setBackgroundColor(float r, float g, float b, float a);
	private:
		void createGLFWContext();
		void initGLFW();
		void configureGLFW();
		void openWindow();
		void initGLAD();
		void setGLFWcallbacks();
		void setGLvalues();
		
		void loadShaders(std::vector<std::string> shadersPaths);
	};
}