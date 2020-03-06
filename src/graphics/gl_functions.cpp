#include "gl_functions.h"
#include "keyboard.h"
#include "utils.h"
#include "shader.h"
#include "camera.h"
#include "transform.h"
#include "mouse.h"
#include "lib/log.h"

namespace gph = graphics;



GLFWwindow * gph::createGLFWContext(int width, int height, std::string name) {
	windowWidth = width;
	windowHeight = height;
  BOOST_LOG_TRIVIAL(trace) << "Attempting to initialise GLFW";
	if (!glfwInit())
	{
    BOOST_LOG_TRIVIAL(error) << "Failed to initialize GLFW";
		exit(-1);
	}
  BOOST_LOG_TRIVIAL(trace) << "Configuring GLFW";
	glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // We want OpenGL 4.0
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL 

  BOOST_LOG_TRIVIAL(trace) << "Open a window and create its OpenGL context";
	GLFWwindow* window = glfwCreateWindow(width, height, name.data(), NULL, NULL);
	if (window == NULL)
	{
    BOOST_LOG_TRIVIAL(error) << "Failed to open GLFW window!";
		exit(-1);
	}
	glfwMakeContextCurrent(window);
  BOOST_LOG_TRIVIAL(trace) << "Attempting to initialise GLAD";
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
    BOOST_LOG_TRIVIAL(error) << "Failed to initialize GLAD";
		exit(-1);
	}
	glClearColor(0, 0, 0, 1.0f);
	glViewport(0, 0, windowWidth, windowHeight);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  BOOST_LOG_TRIVIAL(trace) << "Setting callbacks for GLFW";

	glfwSetScrollCallback(window, scrollCallback);
	glfwSetCursorPosCallback(window, moveCursorCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetKeyCallback(window, keyboardCallback);
	glfwSetWindowSizeCallback(window, windowResizeCallback);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE);

	return window;
}

void gph::windowResizeCallback(GLFWwindow* window, int width, int height) {
  BOOST_LOG_TRIVIAL(trace) << "Window resized";
	windowWidth = width;
	windowHeight = height;
	windowResized = true;
}

void gph::update(GLFWwindow* window, gph::GameObject* mainScene, double& lastTime, double& check, int fps) {
	double currentTime = glfwGetTime();
	deltaTime = currentTime - lastTime;
	lastTime = currentTime;

	if (windowResized) {
		glViewport(0, 0, windowWidth, windowHeight);
		windowResized = false;
	}

	updateCameras(window);
	drawScene(window, mainScene);

	glfwSwapBuffers(window);
	glfwPollEvents();
}


void gph::loadShaders(std::vector<std::string> shadersPath) {
	if (shadersPath.size() % 2) {
		std::cout << "The required shaders must be pairs of vertex and fragment shaders." << std::endl;
		std::cin.ignore();
		exit(-1);
	}
	for (size_t i = 0; i < shadersPath.size(); i += 2) {
		ShaderLoader shaderLoader = ShaderLoader(shadersPath[i], shadersPath[i + 1]);
		std::string name = split(shadersPath[i], '.')[0];
		std::vector<std::string> res = split(name, '/');
		programIDmap[res[res.size() - 1]] = shaderLoader.loadShaders();
	}
}

void gph::updateCameras(GLFWwindow* window) {
	for (auto camera : cameras) {
		for (int i = 0; i < totalCameraMovements; i++) {
			if (camera->moveBuffer[i])
				camera->moveCamera(i);
		}
		if (camera->rotate) {
			camera->rotateCamera(window);
		}
	}
}

void gph::drawScene(GLFWwindow* window, gph::GameObject* mainScene) {
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glUseProgram(programIDmap[MODEL_SHADER]);
	for (auto camera : cameras) {
		glScissor(camera->viewport.x, camera->viewport.y, camera->viewport.width, camera->viewport.height);
		glEnable(GL_SCISSOR_TEST);
		glViewport(camera->viewport.x, camera->viewport.y, camera->viewport.width, camera->viewport.height);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glm::mat4 projection = glm::perspective(glm::radians(cameras[0]->fieldOfView),
			float(camera->viewport.width) / camera->viewport.height,
			cameras[0]->nearClipDistance,
			cameras[0]->farClipDistance);
		glm::mat4 view = glm::lookAt(glm::vec3(cameras[0]->transform->position),
			cameras[0]->lookAt,
			glm::vec3(0, 1, 0));
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 mvp = projection * view * model;
		GLuint matrixID = glGetUniformLocation(programIDmap[MODEL_SHADER], "mvp");
		glUniformMatrix4fv(matrixID, 1, GL_FALSE, &mvp[0][0]);
		glBindVertexArray(vertexArrayID);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}	
}

void gph::drawUI() { }

void gph::cleanup(GameObject* mainScene) {
  BOOST_LOG_TRIVIAL(trace) << "Cleaning up";
	for (auto value: programIDmap) {
    BOOST_LOG_TRIVIAL(trace) << "deleting programID";
		glDeleteProgram(value.second);
	}
  BOOST_LOG_TRIVIAL(trace) << "cleansing programID";
	programIDmap.clear();
	for (auto camera : cameras) {
    BOOST_LOG_TRIVIAL(trace) << "deleting camera";
		delete camera;
	}
  BOOST_LOG_TRIVIAL(trace) << "cleansing cameras";
	cameras.clear();
  BOOST_LOG_TRIVIAL(trace) << "deleting vertex array";
	glDeleteVertexArrays(1, &vertexArrayID);
  BOOST_LOG_TRIVIAL(trace) << "deleting vertex buffer";
	glDeleteBuffers(1, &vertexBuffer);
  BOOST_LOG_TRIVIAL(trace) << "deleting scene";
	delete mainScene;
  BOOST_LOG_TRIVIAL(trace) << "terminating GLFW";
	glfwTerminate();
  BOOST_LOG_TRIVIAL(trace) << "cleanup successful";
}
