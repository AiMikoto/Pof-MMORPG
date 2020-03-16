#pragma once
#include <string>
#include "constants.h"
#include <boost/property_tree/ptree.hpp>
#include "shader.h"

namespace graphics {

	struct Texture {
		uint id, type;
		std::string path;

		Texture();
		Texture(std::string path);
		Texture(std::string path, uint type);
		~Texture();
		boost::property_tree::ptree serialize();
		void deserialize(boost::property_tree::ptree node);
	private:
		void load();
	};

	class Material {
	public:
		uint textureID, shaderID;
		glm::vec4 color;

		Material();
		~Material();
		Material(uint textureID, uint shaderID, glm::vec4 color = gph::colors::gray);

		boost::property_tree::ptree serialize();
		void deserialize(boost::property_tree::ptree node);
		void shaderSetup(float alpha);
	};
}