#include "gl_functions.h"
#include "keyboard_functions.h"
#include "utils.h"

namespace gph = graphics;

GLFWwindow * gph::createGLFWContext(int width, int height, std::string name) {
	if (!glfwInit())
	{
		std::cout << "Failed to initialize GLFW" << std::endl;
		std::cin.ignore();
		exit(-1);
	}
	glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // We want OpenGL 4.0
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL 

	// Open a window and create its OpenGL context
	GLFWwindow* window = glfwCreateWindow(width, height, name.data(), NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to open GLFW window!" << std::endl;
		glfwTerminate();
		std::cin.ignore();
		exit(-1);
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		std::cin.ignore();
		exit(-1);
	}
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);
	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//glfwSetScrollCallback(window, scrollCallback);
	//glfwSetCursorPosCallback(window, moveCursorCallback);
	//glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetKeyCallback(window, keyboardCallback);
	glfwSetWindowSizeCallback(window, windowResizeCallback);

	return window;
}

void gph::windowResizeCallback(GLFWwindow* window, int width, int height) {
	windowWidth = width;
	windowHeight = height;
	windowResized = true;
}

void gph::update(GLFWwindow* window, gph::GameObject* mainScene, float lastTime, float check, int fps) {
	
	glClear(GL_COLOR_BUFFER_BIT);
	
	//we only load 1 program for now, will later declare a shaders class and deal with that
	
	DrawScene(mainScene);

	glfwSwapBuffers(window);
	glfwPollEvents();
}

gph::ShaderLoader::ShaderLoader(std::string vertexShaderPath, std::string fragmentShaderPath) {
	this->vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	this->fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	this->vertexShaderPath = vertexShaderPath;
	this->fragmentShaderPath = fragmentShaderPath;
}

gph::ShaderLoader::~ShaderLoader() {
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
}

std::string gph::ShaderLoader::readShaderFile(std::string path) {
	std::string res;
	std::ifstream fileStream(path, std::ios::in);
	if (fileStream.is_open()) {
		std::string line = "";
		while (std::getline(fileStream, line))
			res += line + "\n";
		fileStream.close();
	}
	else throw "Impossible to open " + path;
	return res;
}

GLuint gph::ShaderLoader::loadShaders() {
	try {
		std::string vertexShaderCode, fragmentShaderCode;
		vertexShaderCode = readShaderFile(this->vertexShaderPath);
		fragmentShaderCode = readShaderFile(this->fragmentShaderPath);

		std::cout << "Compiling vertex shader: " << vertexShaderPath << std::endl;
		compileShader(vertexShaderCode, vertexShaderID);
		std::cout << "Compiling fragment shader: " << fragmentShaderPath << std::endl;
		compileShader(vertexShaderCode, vertexShaderID);

		return linkProgram();
	}
	catch (std::string e) {
		std::cout << e << std::endl;
		std::cin.ignore();
		glDeleteShader(vertexShaderID);
		glDeleteShader(fragmentShaderID);
		exit(1);
	}
	catch (std::vector<char> e) {
		std::cout << &e[0] << std::endl;
	}
}

void gph::ShaderLoader::compileShader(std::string shader, GLuint shaderID) {
	try {
		GLint result = GL_FALSE;
		int infoLogLength;
		char const* sourcePointer = shader.c_str();
		glShaderSource(shaderID, 1, &sourcePointer, NULL);
		glCompileShader(shaderID);
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (infoLogLength > 0) {
			std::vector<char> vertexShaderErrorMessage(infoLogLength + 1);
			glGetShaderInfoLog(vertexShaderID, infoLogLength, NULL, &vertexShaderErrorMessage[0]);
			throw vertexShaderErrorMessage;
		}
	}
	catch (std::vector<char> e) {
		throw e;
	}
}

GLuint gph::ShaderLoader::linkProgram() {
	try {
		std::cout << "Linking program" << std::endl;
		GLint result = GL_FALSE;
		int infoLogLength;
		GLuint programID = glCreateProgram();
		glAttachShader(programID, vertexShaderID);
		glAttachShader(programID, fragmentShaderID);
		glLinkProgram(programID);
		glGetProgramiv(programID, GL_LINK_STATUS, &result);
		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (infoLogLength > 0) {
			std::vector<char> programErrorMessage(infoLogLength + 1);
			glGetProgramInfoLog(programID, infoLogLength, NULL, &programErrorMessage[0]);
			glDetachShader(programID, vertexShaderID);
			glDetachShader(programID, fragmentShaderID);
			throw programErrorMessage;
		}
		glDetachShader(programID, vertexShaderID);
		glDetachShader(programID, fragmentShaderID);
		glDeleteShader(vertexShaderID);
		glDeleteShader(fragmentShaderID);
		return programID;
	}
	catch (std::vector<char> e) {
		throw e;
	}
}

void gph::loadShaders(std::vector<std::string> shadersPath) {
	if (shadersPath.size() % 2) {
		std::cout << "The required shaders must be pairs of vertex and fragment shaders." << std::endl;
		std::cin.ignore();
		exit(-1);
	}
	for (size_t i = 0; i < shadersPath.size(); i += 2) {
		ShaderLoader shaderLoader = ShaderLoader(shadersPath[i], shadersPath[i + 1]);
		shaderProgramsIDs.push_back(shaderLoader.loadShaders());
	}
}

void gph::UpdateCamera(GLFWwindow* window) { }

void gph::DrawScene(gph::GameObject* mainScene) {
	glUseProgram(shaderProgramsIDs[0]);
	glBindVertexArray(vertexArrayID);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

void gph::DrawUI() { }

void gph::Cleanup(GameObject* mainScene) {
	std::cout << "Cleaning up..." << std::endl;
	for (auto programID: shaderProgramsIDs) {
		glDeleteProgram(programID);
	}
	glDeleteVertexArrays(1, &vertexArrayID);
	glDeleteBuffers(1, &vertexBuffer);
	delete mainScene;
}