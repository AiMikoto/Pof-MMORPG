#include "gl_functions.h"
#include "keyboard_functions.h"

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

void gph::update(GLFWwindow* window, float lastTime, float check, int fps) {
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	//we only load 1 program for now, will later declare a shaders class and deal with that
	glUseProgram(shaderProgramsIDs[0]);

	glfwSwapBuffers(window);
	glfwPollEvents();
}

GLuint gph::loadShaders(std::string vertexFilePath, std::string fragmentFilePath)
{
	// Create the shaders
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	// Read the Vertex Shader code from the file
	std::string vertexShaderCode;
	std::ifstream vertexShaderStream(vertexFilePath, std::ios::in);
	if (vertexShaderStream.is_open())
	{
		std::string Line = "";
		while (std::getline(vertexShaderStream, Line))
			vertexShaderCode += "\n" + Line;
		vertexShaderStream.close();
	}
	else
	{
		std::cout << "Impossible to open " << vertexFilePath << std::endl;
		std::cin.ignore();
		exit(1);
	}
	// Read the Fragment Shader code from the file
	std::string fragmentShaderCode;
	std::ifstream fragmentShaderStream(fragmentFilePath, std::ios::in);
	if (fragmentShaderStream.is_open())
	{
		std::string line = "";
		while (std::getline(fragmentShaderStream, line))
			fragmentShaderCode += "\n" + line;
		fragmentShaderStream.close();
	}
	else
	{
		std::cout << "Impossible to open " << fragmentFilePath << std::endl;
		std::cin.ignore();
		exit(1);
	}
	GLint result = GL_FALSE;
	int infoLogLength;
	// Compile Vertex Shader
	std::cout << "Compiling vertex shader: " << vertexFilePath << std::endl;
	char const * vertexSourcePointer = vertexShaderCode.c_str();
	glShaderSource(vertexShaderID, 1, &vertexSourcePointer, NULL);
	glCompileShader(vertexShaderID);
	// Check Vertex Shader
	glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (infoLogLength > 0)
	{
		std::vector<char> vertexShaderErrorMessage(infoLogLength + 1);
		glGetShaderInfoLog(vertexShaderID, infoLogLength, NULL, &vertexShaderErrorMessage[0]);
		std::cout << &vertexShaderErrorMessage[0] << std::endl;
	}
	// Compile Fragment Shader
	std::cout << "Compiling fragment shader: " << fragmentFilePath << std::endl;
	char const * fragmentSourcePointer = fragmentShaderCode.c_str();
	glShaderSource(fragmentShaderID, 1, &fragmentSourcePointer, NULL);
	glCompileShader(fragmentShaderID);
	// Check Fragment Shader
	glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(fragmentShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (infoLogLength > 0)
	{
		std::vector<char> fragmentShaderErrorMessage(infoLogLength + 1);
		glGetShaderInfoLog(fragmentShaderID, infoLogLength, NULL, &fragmentShaderErrorMessage[0]);
		std::cout << &fragmentShaderErrorMessage[0] << std::endl;
	}
	// Link the program
	std::cout << "Linking program" << std::endl;
	GLuint programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glLinkProgram(programID);
	// Check the program
	glGetProgramiv(programID, GL_LINK_STATUS, &result);
	glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (infoLogLength > 0) {
		std::vector<char> programErrorMessage(infoLogLength + 1);
		glGetProgramInfoLog(programID, infoLogLength, NULL, &programErrorMessage[0]);
		std::cout << &programErrorMessage[0] << std::endl;
	}
	glDetachShader(programID, vertexShaderID);
	glDetachShader(programID, fragmentShaderID);
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
	return programID;
}

void gph::loadRequiredShaders(std::vector<std::string> shadersPath) {
	if (shadersPath.size() % 2) {
		std::cout << "The required shaders must be pairs of vertex and fragment shaders." << std::endl;
		std::cin.ignore();
		exit(-1);
	}
	for (size_t i = 0; i < shadersPath.size(); i += 2) {
		shaderProgramsIDs.push_back(loadShaders(shadersPath[i], shadersPath[i + 1]));
	}
}

void gph::UpdateCamera(GLFWwindow* window) { }

void gph::DrawScene(gph::GameObject* mainScene) {

}

void gph::DrawUI() { }

void gph::Cleanup(GameObject* mainScene, GLuint vertexArrayID) {
	std::cout << "Cleaning up..." << std::endl;
	for (auto programID: shaderProgramsIDs) {
		glDeleteProgram(programID);
	}
	glDeleteVertexArrays(1, &vertexArrayID);
}