#include "scene.h"
#include "gl_functions.h"
#include "lib/log.h"
#include <boost/property_tree/json_parser.hpp>

namespace gph = graphics;

gph::Scene* gph::activeScene;
std::map<llong, gph::GameObject*> gph::gameObjects;

gph::Scene::Scene() {
	mDrawObjects = mDrawUI = true;
}

gph::Scene::~Scene() {
	for (auto t : textures) {
		delete t.second;
	}
	textures.clear();
	meshes.clear();
	cameras.clear();
	lights.clear();
	for (auto go : gameObjects) {
		if (this->id != go.second->id)
			delete go.second;
	}
	gameObjects.clear();
}

void gph::Scene::update(GLFWwindow* window) {
	updateGameObjects(window);
	updateCameras(window);
	draw(window);
}

void gph::Scene::draw(GLFWwindow* window) {
	if(mDrawObjects) drawObjects(window);
	if(mDrawUI) drawUI(window);
}

void gph::Scene::drawObjects(GLFWwindow* window) {
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	Shader* current = shaderMap[shaderTypes::modelShader];
	current->use();
#pragma omp parralel for
	for (auto camera : cameras) {
		camera.second->setViewport(window);
		setBackground();
		for (auto mesh : meshes) {
			mesh.second->draw(current, camera.second, window);
		}
	}
}

void gph::Scene::drawUI(GLFWwindow* window) {}

void gph::Scene::setBackground() {
	setBackgroundColor(colors::bgColor);
}

void gph::Scene::updateCameras(GLFWwindow* window) {
	for (auto camera : cameras) {
		for (int i = 0; i < totalCameraMovements; i++) {
			if (camera.second->moveBuffer[i])
				camera.second->moveCamera(i);
		}
		if (camera.second->rotate) {
			camera.second->rotateCamera(window);
		}
	}
}

void gph::Scene::updateGameObjects(GLFWwindow* window) {
	for (auto i : childrenIDs) {
		gameObjects[i]->update(window);
	}
}

void gph::Scene::addCamera(Camera* camera, bool isMainCamera) {
	addChild(camera->id);
	cameras[camera->id] = camera;
	if(isMainCamera) mainCameraID = camera->id;
}

void gph::Scene::addMesh(Mesh* mesh) {
	addChild(mesh->id);
	meshes[mesh->id] = mesh;
}

void gph::Scene::sceneToJSON(std::string path) {
	boost::property_tree::ptree root = serialize();
	boost::property_tree::write_json(path, root);
}

void gph::Scene::sceneFromJSON(std::string data) {}

boost::property_tree::ptree gph::Scene::serialize() {
	boost::property_tree::ptree root, cNode, tNode, mNode, goNode;
	std::map<llong, GameObject*> gameObjectsCopy;
	for (auto g : gameObjects) {
		gameObjectsCopy[g.second->id] = g.second;
	}
	for (auto camera : cameras) {
		boost::property_tree::ptree camNode = camera.second->serialize();
		cNode.push_back(std::pair<std::string, boost::property_tree::ptree>("", camNode));
		gameObjectsCopy.erase(camera.second->id);
	}
	for (auto texture : textures) {
		tNode.push_back(std::pair<std::string, boost::property_tree::ptree>("", texture.second->serialize()));
	}
	for (auto mesh : meshes) {
		mNode.push_back(std::pair<std::string, boost::property_tree::ptree>("", mesh.second->serialize()));
		gameObjectsCopy.erase(mesh.second->id);
	}
	for (auto g : gameObjectsCopy) {
		goNode.push_back(std::pair<std::string, boost::property_tree::ptree>("", g.second->serialize()));
	}
	root.add_child("Cameras", cNode);
	root.add_child("Textures", tNode);
	root.add_child("Meshes", mNode);
	root.add_child("Unaccounted Game Objects", goNode);
	return root;
}

void deserialize(boost::property_tree::ptree node) {

}