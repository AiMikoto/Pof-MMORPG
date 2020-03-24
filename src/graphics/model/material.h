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
		~Texture();
		boost::property_tree::ptree serialize();
		void deserialize(boost::property_tree::ptree node);
	private:
		void load();
	};

	struct Material {
		uint shaderID;
		glm::vec4 colorDiffuse, colorSpecular, colorAmbient, colorEmissive, colorTransparent;
		bool twoSided;
		int blend, shading;
		float opacity, shininess, shininessStrength;
		std::vector<uint> texturesIDs;
		std::vector<int> texturesOP;
		std::vector<float> texturesStrength;
		std::string path;

		Material();
		~Material();
		Material(uint shaderID);

		boost::property_tree::ptree serialize();
		void deserialize(boost::property_tree::ptree node);
		void contextSetup();
	};
}