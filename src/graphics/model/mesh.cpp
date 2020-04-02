#include "graphics/model/mesh.h"
#include "scene/transform.h"
#include "lib/log.h"
#include "scene/scene.h"
#include "core/utils.h"
#include "core/constants.h"

engine::Mesh::Mesh(std::vector<Vertex> vertices, std::string materialPath, std::vector<uint> indices) {
	this->vertices = vertices;
	this->materialPath = materialPath;
	this->indices = indices;
}

engine::Mesh::~Mesh() {
	vertices.clear();
	indices.clear();
}

void engine::Mesh::computeScale(){
	glm::dvec3 min = vertices[0].position;
	glm::dvec3 max = vertices[0].position;
	for (auto v : vertices) {
		min.x = glm::min(min.x, double(v.position.x));
		min.y = glm::min(min.y, double(v.position.y));
		min.z = glm::min(min.z, double(v.position.z));
		max.x = glm::max(max.x, double(v.position.x));
		max.y = glm::max(max.y, double(v.position.y));
		max.z = glm::max(max.z, double(v.position.z));
	}
	meshScale = max - min;
	BOOST_LOG_TRIVIAL(trace) << meshScale.x << ", " << meshScale.y << ", " << meshScale.z;
}