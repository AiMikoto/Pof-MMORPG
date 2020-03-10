#include "shader.h"
#include "lib/log.h"

namespace gph = graphics;

std::map<std::string, gph::Shader*> gph::shaderMap;

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

		BOOST_LOG_TRIVIAL(trace) << "Compiling vertex shader: " << vertexShaderPath;
		compileShader(vertexShaderCode, vertexShaderID);
		BOOST_LOG_TRIVIAL(trace) << "Compiling fragment shader: " << fragmentShaderPath;
		compileShader(fragmentShaderCode, fragmentShaderID);
		BOOST_LOG_TRIVIAL(trace) << "Linking program";
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

gph::Shader::Shader(std::string vertexShaderPath, std::string fragmentShaderPath) {
	load(vertexShaderPath, fragmentShaderPath);
}

gph::Shader::~Shader() {
	glDeleteProgram(programID);
}

void gph::Shader::load(std::string vertexShaderPath, std::string fragmentShaderPath) {
	ShaderLoader loader(vertexShaderPath, fragmentShaderPath);
	programID = loader.loadShaders();
}

void gph::Shader::use() {
	glUseProgram(programID);
}

void gph::Shader::draw() {}

void gph::Shader::setBool(std::string name, bool value) {
	glUniform1i(glGetUniformLocation(programID, name.c_str()), (int)value);
}

void gph::Shader::setInt(std::string name, int value) {
	glUniform1i(glGetUniformLocation(programID, name.c_str()), value);
}

void gph::Shader::setFloat(std::string name, float value) {
	glUniform1f(glGetUniformLocation(programID, name.c_str()), value);
}

void gph::Shader::setVec2(std::string name, glm::vec2 value) {
	glUniform2fv(glGetUniformLocation(programID, name.c_str()), 1, &value[0]);
}

void gph::Shader::setVec2(std::string name, float x, float y) {
	glUniform2f(glGetUniformLocation(programID, name.c_str()), x, y);
}

void gph::Shader::setVec3(std::string name, glm::vec3 value) {
	glUniform3fv(glGetUniformLocation(programID, name.c_str()), 1, &value[0]);
}

void gph::Shader::setVec3(std::string name, float x, float y, float z) {
	glUniform3f(glGetUniformLocation(programID, name.c_str()), x, y, z);
}

void gph::Shader::setVec4(std::string name, glm::vec4 value) {
	glUniform4fv(glGetUniformLocation(programID, name.c_str()), 1, &value[0]);
}

void gph::Shader::setVec4(std::string name, float x, float y, float z, float w) {
	glUniform4f(glGetUniformLocation(programID, name.c_str()), x, y, z, w);
}

void gph::Shader::setMat2(std::string name, glm::mat2 mat) {
	glUniformMatrix2fv(glGetUniformLocation(programID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void gph::Shader::setMat3(std::string name, glm::mat3 mat) {
	glUniformMatrix3fv(glGetUniformLocation(programID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void gph::Shader::setMat4(std::string name, glm::mat4 mat) {
	glUniformMatrix4fv(glGetUniformLocation(programID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}