#pragma once
#include <boost/property_tree/ptree.hpp>
#include "glm/gtc/type_ptr.hpp"
#include <vector>
#include <string>
#include "core/constants.h"

namespace engine {
	struct Vertex {
		glm::vec3 position, normal, tangent, bitangent;
		glm::vec2 textureCoordinates;
	};	

	struct Mesh {
		std::vector<Vertex> vertices;
		std::string materialPath;
		std::vector<uint> indices;
		glm::dvec3 meshScale;
		std::string name;

		Mesh(std::vector<Vertex> vertices, std::string materialPath, std::vector<uint> indices);
		~Mesh();
		void computeScale();	
	};
}