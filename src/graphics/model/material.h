#pragma once
#include <string>
#include "core/constants.h"
#include <boost/property_tree/ptree.hpp>

namespace engine {
	struct Texture {
		uint id, type;
		std::string path;

		Texture();
		Texture(std::string path);
		Texture(std::string path, uint type);
		Texture(boost::property_tree::ptree node);
		~Texture();
		boost::property_tree::ptree serialize();
	private:
		void load();
	};

	struct Material {
		uint shaderType;
		glm::vec4 colorDiffuse, colorSpecular, colorAmbient, colorEmissive, colorTransparent;
		bool twoSided;
		int blend, shading;
		float opacity, shininess, shininessStrength;
		std::vector<std::string> texturesPaths;
		std::vector<int> texturesOP;
		std::vector<float> texturesStrength;
		std::string path;

		Material();
		~Material();
		Material(uint shaderType);
		Material(boost::property_tree::ptree node);
		boost::property_tree::ptree serialize();
		void contextSetup();
		void writeToFile();
	private:
		void load();
	};
}