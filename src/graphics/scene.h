#pragma once
#include "graphics_files.h"
#include "objects.h"
#include "transform.h"
#include "shader.h"
#include "camera.h"
#include "light.h"
#include "model.h"
#include <map>
#include <vector>
#include <string>
#include <boost/property_tree/ptree.hpp>

namespace graphics {
	class Scene : public GameObject {
	public:
		std::map<uint, Texture*> textures;
		std::map<llong, Mesh*> meshes;
		std::map<llong, Camera*> cameras;
		std::map<llong, Light*> lights;
		
		bool mDrawObjects, mDrawUI;
		llong mainCameraID;

		Scene();
		~Scene();
		void update(GLFWwindow* window);
		void setBackground();
		void addCamera(Camera* camera, bool isMainCamera = false);
		void addMesh(Mesh* mesh);
		void sceneToJSON(std::string path);
		void sceneFromJSON(std::string data);
		boost::property_tree::ptree serialize();
		void deserialize(boost::property_tree::ptree node);
	private:
		void draw(GLFWwindow* window);
		void drawObjects(GLFWwindow* window);
		void drawUI(GLFWwindow* window);
		void updateCameras(GLFWwindow* window);
		void updateGameObjects(GLFWwindow* window);
	};

	extern Scene* activeScene;
	extern std::map<llong, GameObject*> gameObjects;
}