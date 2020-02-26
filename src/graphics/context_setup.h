#pragma once
#include "graphics_files.h"
#include <string>
#include <iostream>

namespace graphics {
	namespace setup {
		void setup(int width, int height, std::string name);
		void loadGlad();
		void createGLFWcontext();
		void framebufferSizeCallback(GLFWwindow* window, int width, int height);
	}
}