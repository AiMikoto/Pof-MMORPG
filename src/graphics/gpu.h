#pragma once
#include <map>
#include "graphics/shader.h"
#include "graphics/model/material.h"
#include "graphics/gl_context.h"
#include "components/camera.h"
#include "components/meshRenderer.h"
#include "scene/scene.h"
#include "graphics/model/mesh.h"
#include "components/meshFilter.h"
#include "graphics/model/model.h"
#include <boost/property_tree/ptree.hpp>
#include "graphics/renderLayer.h"
#include "lib/nuklear.h"
#include "ui/ui.h"

namespace engine {
	class GPU {
	public:
		std::map<std::string, Texture*> textures;
		std::map<uint, Shader*> shaders;
		std::map<std::string, Material*> materials;
		std::map<std::string, Model*> models;
		std::vector<Camera*> cameras;
		//optimizing objects that use the same material not to switch context on the gpu
		
		std::vector<Scene*> activeScenes;
		GLContext* glContext;
		Camera* editorCamera;
		ModelLoader* modelLoader;
		struct nk_context *ctx = NULL;
		UI_master *ui = NULL;

		GPU();
		~GPU();
		void initializeContext();
		void initializeGUI();
		void addUI(UI_master *ui);
		void draw();
		void update();
		void addRenderer(MeshRenderer* renderer);
		void removeRenderer(MeshRenderer* renderer);
		void removeModel(std::string path);
	private:
		std::map<std::string, std::map<std::string, RenderLayer*>> renderLayers;
		void drawScene();
		void drawUI();
	};

	inline GPU* gpu = NULL;
}
