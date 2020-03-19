#include "mesh.h"
#include "transform.h"
#include "lib/log.h"
#include "scene.h"
#include "utils.h"
#include "constants.h"

engine::Mesh::Mesh(std::vector<Vertex> vertices, uint materialID, std::vector<uint> indices) {
	this->vertices = vertices;
	this->materialID = materialID;
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
		if (v.position.x < min.x)
			min.x = v.position.x;
	}
	meshScale = max - min;
	BOOST_LOG_TRIVIAL(trace) << meshScale.x << ", " << meshScale.y << ", " << meshScale.z;
}

boost::property_tree::ptree engine::Vertex::serialize() {
	boost::property_tree::ptree node;
	node.add_child("position", dvec3serializer(position));
	node.add_child("normal", dvec3serializer(normal));
	node.add_child("textureCoordinates", dvec2serializer(textureCoordinates));
	node.add_child("tangent", dvec3serializer(tangent));
	node.add_child("bitangent", dvec3serializer(bitangent));
	return node;
}

void engine::Vertex::deserialize(boost::property_tree::ptree node) {

}

boost::property_tree::ptree engine::Mesh::serialize() {
	boost::property_tree::ptree node, verticesNode;
	for (int i = 0; i < vertices.size(); i++) {
		verticesNode.add_child("v" + std::to_string(i), vertices[i].serialize());
	}
	node.add_child("vertices", verticesNode);
	node.put("materialID", materialID);
	node.put("indices", vectorToString(indices, ' '));
	return node;
}

void engine::Mesh::deserialize(boost::property_tree::ptree node) {
	boost::property_tree::ptree verticesNode;
}

