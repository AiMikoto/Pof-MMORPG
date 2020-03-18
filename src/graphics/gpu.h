#pragma once
#include <map>
#include "constants.h"
#include "shader.h"
#include "material.h"
#include "camera.h"
#include "mesh.h"
#include "scene.h"

namespace graphics {
	class GPU {
	public:
		std::map<uint, Texture*> textures;
		std::map<uint, Shader*> shaders;
		std::map<uint, Material*> materials;
		std::map<uint, Mesh*> meshes;
		std::vector<Camera*> cameras;
		//optimizing objects that use the same material not to switch context on the gpu
		std::map<uint, glm::ivec2> sortedRendererMat;
		std::vector<Scene*> activeScenes;
		GLFWwindow* window;

		GPU(GLFWwindow* window);
		~GPU();
		void draw();
		void update();
		void addRenderer(MeshRenderer* renderer);
		void removeRenderer(MeshRenderer* renderer);
	private:
		void sortRenderers();
		std::vector<MeshRenderer*> renderers;
		
	};

	extern GPU* gpu;
}