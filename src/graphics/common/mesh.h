#pragma once
#include <boost/property_tree/ptree.hpp>
#include "glm/gtc/type_ptr.hpp"
#include <vector>
#include <string>
#include "constants.h"

namespace engine {
	struct Vertex {
		glm::vec3 position, normal, tangent, bitangent;
		glm::vec2 textureCoordinates;
		boost::property_tree::ptree serialize();
		void deserialize(boost::property_tree::ptree node);
	};	

	struct Mesh {
		std::vector<Vertex> vertices;
		uint materialID;
		std::vector<uint> indices;
		glm::dvec3 meshScale;
		std::string path;

		Mesh(std::vector<Vertex> vertices, uint materialID, std::vector<uint> indices);
		~Mesh();
		boost::property_tree::ptree serialize();
		void deserialize(boost::property_tree::ptree node);
		void computeScale();	
	};
}