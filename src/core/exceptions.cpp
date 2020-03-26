#include "core/exceptions.h"

engine::shader_compile_error::shader_compile_error(std::string info) {
	this->info = info;
}

const char* engine::shader_compile_error::what() {
	return info.c_str();
}

engine::missing_component::missing_component(std::string type) {
	this->type = type;
}

const char* engine::missing_component::what() {
	return ("Missing component of type: " + type).c_str();
}

engine::assimp_error::assimp_error(std::string info) {
	this->info = info;
}

const char* engine::assimp_error::what() {
	return info.c_str();
}