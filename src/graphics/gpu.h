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

namespace engine {
	class GPU {
	public:
		std::map<uint, Texture*> textures;
		std::map<uint, Shader*> shaders;
		std::map<uint, Material*> materials;
		std::map<std::string, uint> modelsPaths;
		std::map<uint, Model*> models;
		std::vector<Camera*> cameras;
		//optimizing objects that use the same material not to switch context on the gpu
		
		std::vector<Scene*> activeScenes;
		GLContext* glContext;
		Camera* editorCamera;
		ModelLoader* meshLoader;

		GPU();
		~GPU();
		void initializeContext();
		void draw();
		void update();
		void addRenderer(MeshRenderer* renderer);
		void removeRenderer(MeshRenderer* renderer);
		void removeModel(uint id);
		boost::property_tree::ptree serialize();
		static GPU* deserialize(boost::property_tree::ptree node);
		void saveToJSON(std::string path);
		void loadFromJSON(std::string path);
	private:
		std::map<uint, std::map<uint, RenderLayer*>> renderLayers;
		void drawScene();
		void drawUI();
	};

	inline GPU* gpu = NULL;
}