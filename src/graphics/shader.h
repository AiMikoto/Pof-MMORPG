#pragma once
#include "include/glad.h"
#include "include/glfw3.h"
#include <string>
#include <glm/glm.hpp>
#include <map>
#include <iostream>
#include <fstream>

namespace engine {
	struct ShaderLoader {
		GLuint vertexShaderID, fragmentShaderID;
		std::string vertexShaderPath, fragmentShaderPath;

		ShaderLoader(std::string vertexShaderPath, std::string fragmentShaderPath);
		~ShaderLoader();
		std::string readShaderFile(std::string path);
		GLuint loadShaders();
		void compileShader(std::string shader, GLuint shaderID);
		GLuint linkProgram();
	};

	struct ShaderException : std::exception {
		std::string info;

		ShaderException(std::string info);
	};

	class Shader {
	public:
		GLuint programID;
		std::string vertexShaderPath, fragmentShaderPath;
		
		Shader();
		Shader(std::string vertexShaderPath, std::string fragmentShaderPath);
		~Shader();		
		void use();
		void draw();
		void setBool(std::string name, bool value);
		void setInt(std::string name, int value);
		void setFloat(std::string name, float value);
		void setVec2(std::string name, glm::vec2 value);
		void setVec2(std::string name, float x, float y);
		void setVec3(std::string name, glm::vec3 value);
		void setVec3(std::string name, float x, float y, float z);
		void setVec4(std::string name, glm::vec4 value);
		void setVec4(std::string name, float x, float y, float z, float w);
		void setMat2(std::string name, glm::mat2 mat);
		void setMat3(std::string name, glm::mat3 mat);
		void setMat4(std::string name, glm::mat4 mat);
	private:
		void load(std::string vertexShaderPath, std::string fragmentShaderPath);
	};
}