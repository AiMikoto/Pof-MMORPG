#include "shader.h"
#include "lib/log.h"
#include "core/exceptions.h"
#include <iostream>
#include <fstream>

engine::ShaderLoader::ShaderLoader(std::string vertexShaderPath, std::string fragmentShaderPath) {
	this->vertexShaderPath = vertexShaderPath;
	this->fragmentShaderPath = fragmentShaderPath;
}

engine::ShaderLoader::~ShaderLoader() {
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
}

std::string engine::ShaderLoader::readShaderFile(const std::string& path) {
	std::string res;
	std::ifstream fileStream(path, std::ios::in);
	if (fileStream.is_open()) {
		std::string line = "";
		while (std::getline(fileStream, line))
			res += line + "\n";
		fileStream.close();
		return res;
	}
	throw std::system_error(errno, std::system_category(), "Failed to open file at: " + path);	
}

GLuint engine::ShaderLoader::loadShaders() {
	try {
		std::string vertexShaderCode, fragmentShaderCode;
		vertexShaderCode = readShaderFile(this->vertexShaderPath);
		fragmentShaderCode = readShaderFile(this->fragmentShaderPath);
		vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

		BOOST_LOG_TRIVIAL(trace) << "Compiling vertex shader: " << vertexShaderPath;
		compileShader(vertexShaderCode, vertexShaderID);
		BOOST_LOG_TRIVIAL(trace) << "Compiling fragment shader: " << fragmentShaderPath;
		compileShader(fragmentShaderCode, fragmentShaderID);
		BOOST_LOG_TRIVIAL(trace) << "Linking program";
		return linkProgram();
	}
	catch (std::system_error e) {
		BOOST_LOG_TRIVIAL(error) << e.what() << ", " << e.code();
		std::cin.ignore();
		exit(1);
	}
	catch (shader_compile_error e){
		BOOST_LOG_TRIVIAL(error) << e.what();
		glDeleteShader(vertexShaderID);
		glDeleteShader(fragmentShaderID);
		std::cin.ignore();
		exit(1);
	}
}

void engine::ShaderLoader::compileShader(std::string shader, GLuint shaderID) {
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
		throw shader_compile_error(std::string(shaderErrorMessage.begin(), shaderErrorMessage.end()));
	}
}

GLuint engine::ShaderLoader::linkProgram() {
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
		throw shader_compile_error(std::string(programErrorMessage.begin(), programErrorMessage.end()));
	}
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
	return programID;
}

engine::Shader::Shader() {}

engine::Shader::Shader(std::string vertexShaderPath, std::string fragmentShaderPath) {
	this->vertexShaderPath = vertexShaderPath;
	this->fragmentShaderPath = fragmentShaderPath;
	load(vertexShaderPath, fragmentShaderPath);
}

engine::Shader::~Shader() {
	glDeleteProgram(programID);
}

void engine::Shader::load(std::string vertexShaderPath, std::string fragmentShaderPath) {
	ShaderLoader loader(vertexShaderPath, fragmentShaderPath);
	programID = loader.loadShaders();
}

void engine::Shader::use() {
	glUseProgram(programID);
}

void engine::Shader::draw() {}

void engine::Shader::setBool(std::string name, bool value) {
	glUniform1i(glGetUniformLocation(programID, name.c_str()), (int)value);
}

void engine::Shader::setInt(std::string name, int value) {
	glUniform1i(glGetUniformLocation(programID, name.c_str()), value);
}

void engine::Shader::setFloat(std::string name, float value) {
	glUniform1f(glGetUniformLocation(programID, name.c_str()), value);
}

void engine::Shader::setVec2(std::string name, glm::vec2 value) {
	glUniform2fv(glGetUniformLocation(programID, name.c_str()), 1, &value[0]);
}

void engine::Shader::setVec2(std::string name, float x, float y) {
	glUniform2f(glGetUniformLocation(programID, name.c_str()), x, y);
}

void engine::Shader::setVec3(std::string name, glm::vec3 value) {
	glUniform3fv(glGetUniformLocation(programID, name.c_str()), 1, &value[0]);
}

void engine::Shader::setVec3(std::string name, float x, float y, float z) {
	glUniform3f(glGetUniformLocation(programID, name.c_str()), x, y, z);
}

void engine::Shader::setVec4(std::string name, glm::vec4 value) {
	glUniform4fv(glGetUniformLocation(programID, name.c_str()), 1, &value[0]);
}

void engine::Shader::setVec4(std::string name, float x, float y, float z, float w) {
	glUniform4f(glGetUniformLocation(programID, name.c_str()), x, y, z, w);
}

void engine::Shader::setMat2(std::string name, glm::mat2 mat) {
	glUniformMatrix2fv(glGetUniformLocation(programID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void engine::Shader::setMat3(std::string name, glm::mat3 mat) {
	glUniformMatrix3fv(glGetUniformLocation(programID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void engine::Shader::setMat4(std::string name, glm::mat4 mat) {
	glUniformMatrix4fv(glGetUniformLocation(programID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}