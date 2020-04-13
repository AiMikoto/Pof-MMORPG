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
	std::vector<glm::dvec3> points;
	for (auto v : vertices) {
		points.push_back(v.position);
	}
	meshScale = computeSize(points);
}