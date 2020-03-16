#pragma once
#include <map>
#include "constants.h"
#include "shader.h"
#include "material.h"
#include "camera.h"

namespace graphics {
	class GPU {
	public:
		std::map<uint, Texture*> textures;
		std::map<uint, Shader*> shaders;
		std::map<uint, Material*> materials;
		std::vector<Camera*> cameras;

		GPU();
		~GPU();

		void draw(GLFWwindow* window);
	};

	extern GPU* gpu;
}