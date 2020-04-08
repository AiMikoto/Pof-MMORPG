#pragma once
#include <exception>
#include <string>

namespace engine {
	class shader_compile_error : public std::exception {
	public:
		std::string info;
		shader_compile_error(std::string info);
		const char* what();
	};

	class missing_component : public std::exception {
	public:
		std::string type;
		missing_component(std::string type);
		const char* what();
	};

	class assimp_error : public std::exception {
	public:
		std::string info;
		assimp_error(std::string info);
		const char* what();		
	};
}