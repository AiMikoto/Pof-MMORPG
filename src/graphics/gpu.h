#pragma once
#include <map>
#include "graphics/shader.h"
#include "graphics/model/material.h"
#include "graphics/gl_context.h"
#include "components/camera.h"
#include "components/meshRenderer.h"
#include "scene/scene.h"
#include "graphics/model/mesh.h"

namespace engine {
	class GPU {
	public:
		std::map<uint, Texture*> textures;
		std::map<uint, Shader*> shaders;
		std::map<uint, Material*> materials;
		std::map<uint, Mesh*> meshes;
		std::vector<Camera*> cameras;
		//optimizing objects that use the same material not to switch context on the gpu
		
		std::vector<Scene*> activeScenes;
		GLContext* glContext;
		Camera* editorCamera;

		GPU();
		~GPU();
		void initializeContext();
		void draw();
		void update();
		void addRenderer(MeshRenderer* renderer);
		void removeRenderer(MeshRenderer* renderer);
	private:
		std::map<uint, uint> renderLayersMap;
		std::map<uint, std::vector<MeshRenderer*>> renderLayers;
		void determineRenderLayers(MeshRenderer* renderer);
		void drawScene();
		void drawUI();
	};

	extern GPU* gpu;
}