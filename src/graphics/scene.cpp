#include "scene.h"
#include "gl_functions.h"
#include "lib/log.h"

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
	add_child(camera->id);
	cameras[camera->id] = camera;
	if(isMainCamera) mainCameraID = camera->id;
}

void gph::Scene::addMesh(Mesh* mesh) {
	add_child(mesh->id);
	meshes[mesh->id] = mesh;
}

std::string gph::Scene::sceneToJSON() {

}

void gph::Scene::sceneFromJSON(std::string data) {}