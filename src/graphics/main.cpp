#include "graphics/gl_context.h"
#include "core/utils.h"
#include "components/camera.h"
#include "lib/log.h"
#include "graphics/model/mesh.h"
#include "scene/scene.h"
#include "graphics/gpu.h"
#include "graphics/model/model.h"
#include "components/meshRenderer.h"
#include "components/meshFilter.h"
#include "lib/nuklear.h"
#include "ui/scene_viewer.h"
#include "ui/ui.h"
#include "graphics/culling/frustrumCulling.h"

int main() {
	log_init("graphics");
	BOOST_LOG_TRIVIAL(trace) << "Initializing GPU context";
	engine::gpu = new engine::GPU();
	engine::gpu->initializeContext();
	BOOST_LOG_TRIVIAL(trace) << "GPU context initialized";

	BOOST_LOG_TRIVIAL(trace) << "Creating scene";
	engine::Scene* scene = new engine::Scene();
	engine::gpu->activeScenes.push_back(scene);

	std::string modelPath = "../src/graphics/assets/objects/kaguya.obj";
	std::string defaultModelPath = "../src/graphics/assets/objects/cube.obj";
	engine::gpu->modelLoader->loadModel(modelPath);
	engine::gpu->modelLoader->loadModel(defaultModelPath);
	
	BOOST_LOG_TRIVIAL(trace) << glfwGetTime();
	for (int i = 0; i < 30; i++) {
		for (int j = 0; j < 30; j++) {
			engine::GameObject* model = new engine::GameObject();
			model->transform.position = glm::dvec3(2 * i - 30, 0, 2 * j - 30);
			model->transform.rotateBy(glm::dvec3(12 * (i - 15), 12 * (i + j - 30), 12 * (j - 15)));
			model->transform.scale = glm::dvec3(1, 1, 1);
			model->addComponent(new engine::MeshFilter(modelPath));
			model->addComponent(new engine::MeshRenderer());
			model->isStatic = true;
			scene->addGameObject(model);
		}
	}
	// scene->readFromFile("../src/graphics/assets/scenes/test.json");
	BOOST_LOG_TRIVIAL(trace) << scene->children.size();
	BOOST_LOG_TRIVIAL(trace) << glfwGetTime();
	engine::ViewFrustrum vf;
	vf.computePoints(engine::gpu->glContext->window, engine::gpu->editorCamera);
	vf.computeNormals(engine::gpu->editorCamera);
	glm::dvec3 testPoint = engine::gpu->editorCamera->gameObject->transform.position +
		engine::gpu->editorCamera->forward() * double(engine::gpu->editorCamera->nearClipDistance);
	testPoint += 2.0 * engine::gpu->editorCamera->forward();
	BOOST_LOG_TRIVIAL(trace) << vf.pointInsideFrustrum(testPoint);
	BOOST_LOG_TRIVIAL(trace) << vf.pointInsideFrustrum(testPoint - 5.0 * engine::gpu->editorCamera->forward());
	glfwSwapInterval(0);
	UI_master *ui = new UI_master();
	engine::gpu->addUI(ui);
	ui->insert(new UI_scene_viewer(&scene));
	BOOST_LOG_TRIVIAL(trace) << "Starting renderer";
	while (!engine::gpu->glContext->quit) {
		engine::gpu->update();
	}
	scene->writeToFile("../src/graphics/assets/scenes/test.json");
	delete engine::gpu;
	//delete scene;
	return 0;
}
