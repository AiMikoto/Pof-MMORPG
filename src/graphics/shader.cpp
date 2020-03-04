#include "shader.h"

namespace gph = graphics;

std::map<std::string, GLuint> gph::programIDmap;

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
	else {
		throw ShaderException(path);
	}
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
		compileShader(fragmentShaderCode, fragmentShaderID);
		std::cout << "Linking program" << std::endl;
		return linkProgram();
	}
	catch (ShaderException e) {
		std::cout << e.info << std::endl;
		std::cin.ignore();
		glDeleteShader(vertexShaderID);
		glDeleteShader(fragmentShaderID);
		exit(1);
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
		if (!result) {
			glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
			std::vector<char> shaderErrorMessage(infoLogLength + 1);
			glGetShaderInfoLog(shaderID, infoLogLength, NULL, &shaderErrorMessage[0]);
			throw ShaderException(std::string(shaderErrorMessage.begin(), shaderErrorMessage.end()));
		}
	}
	catch (ShaderException e) {
		throw e;
	}
}

GLuint gph::ShaderLoader::linkProgram() {
	try {
		GLint result = GL_FALSE;
		int infoLogLength;
		GLuint programID = glCreateProgram();
		glAttachShader(programID, vertexShaderID);
		glAttachShader(programID, fragmentShaderID);
		glLinkProgram(programID);
		glGetProgramiv(programID, GL_LINK_STATUS, &result);
		if (!result) {
			glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
			std::vector<char> programErrorMessage(infoLogLength + 1);
			glGetProgramInfoLog(programID, infoLogLength, NULL, &programErrorMessage[0]);
			throw ShaderException(std::string(programErrorMessage.begin(), programErrorMessage.end()));
		}
		glDeleteShader(vertexShaderID);
		glDeleteShader(fragmentShaderID);
		return programID;
	}
	catch (ShaderException e) {
		throw e;
	}
}

gph::ShaderException::ShaderException(std::string info) {
	this->info = info;
}

gph::Shader::Shader() {}

gph::Shader::~Shader() {}

void gph::Shader::draw() {}

void gph::Shader::setUniform(const GLchar* uniformName, GLuint programID, float set) {}

void gph::Shader::setUniform(const GLchar* uniformName, GLuint programID, glm::vec3 set) {}

void gph::Shader::setUniform(const GLchar* uniformName, GLuint programID, glm::vec4 set) {}

void gph::Shader::setUniform(const GLchar* uniformName, GLuint programID, glm::mat4 set) {}