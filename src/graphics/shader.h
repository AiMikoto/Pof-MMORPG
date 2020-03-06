#pragma once
#include "graphics_files.h"
#include <string>
#include <glm/glm.hpp>
#include <map>
#include <iostream>
#include <fstream>
#include "constants.h"
#include "variables.h"

namespace graphics {
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

	struct Shader {
		Shader();
		~Shader();
		void draw();
		void setUniform(const GLchar* uniformName, GLuint programID, float set);
		void setUniform(const GLchar* uniformName, GLuint programID, glm::vec3 set);
		void setUniform(const GLchar* uniformName, GLuint programID, glm::vec4 set);
		void setUniform(const GLchar* uniformName, GLuint programID, glm::mat4 set);
	};

	struct ShaderException : std::exception {
		std::string info;

		ShaderException(std::string info);
	};

	extern std::map<std::string, GLuint> programIDmap;
}