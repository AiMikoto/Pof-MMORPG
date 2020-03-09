#include "model.h"
#include "transform.h"
#include "light.h"

namespace gph = graphics;

gph::Mesh::Mesh(std::vector<Vertex> vertices, std::vector<Texture> textures, std::vector<uint> indices) {
	this->vertices = vertices;
	this->textures = textures;
	this->indices = indices;
	setup();
}

gph::Mesh::~Mesh() {
	glDeleteVertexArrays(1, &vertexArrayID);
	glDeleteBuffers(1, &vertexBufferID);
	glDeleteBuffers(1, &elementsBufferID);
	glDeleteBuffers(1, &outlineIndicesBufferID);
	vertices.clear();
	textures.clear();
	indices.clear();
	outlineIndices.clear();
}

void gph::Mesh::draw(Shader* shader, bool drawOutline) {

}

void gph::Mesh::copy(Mesh* target) {}

void gph::Mesh::setup() {
	bindBuffers();
	createOutline();
}

void gph::Mesh::bindBuffers() {
	glGenVertexArrays(1, &vertexArrayID);
	glGenBuffers(1, &vertexBufferID);
	glGenBuffers(1, &elementsBufferID);
	glGenBuffers(1, &outlineIndicesBufferID);

	glBindVertexArray(vertexArrayID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementsBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint), &indices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, outlineIndicesBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint), &outlineIndices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, textureCoordinates));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));

	glBindVertexArray(0);
}

void gph::Mesh::createOutline() {}

