#include "model.h"
#include "transform.h"
#include "light.h"
#define STB_IMAGE_IMPLEMENTATION
#include "lib/stb_image.h"
#include "lib/log.h"

namespace gph = graphics;

std::vector<gph::Texture*> gph::textures;

gph::Texture::Texture() {}

gph::Texture::Texture(std::string path) {
	this->path = path;
	load();
}

gph::Texture::~Texture() {
	glDeleteTextures(1, &id);
}

void gph::Texture::load() {
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	uchar* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		BOOST_LOG_TRIVIAL(trace) << "Failed to load texture at: " << path;
	}
	stbi_image_free(data);
}

gph::Mesh::Mesh(std::vector<Vertex> vertices, std::vector<uint> textureIndices, std::vector<uint> indices) {
	this->vertices = vertices;
	this->textureIndices = textureIndices;
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

void gph::Mesh::draw(Shader* shader) {
	uint currentDiffuse = 1;
	uint currentSpecular = 1;
	uint currentNormal = 1;
	uint currentHeight = 1;
	int index = 0;
	for (auto i: this->textureIndices) {
		glActiveTexture(GL_TEXTURE0 + i);
		std::string current;
		std::string type = textures[i]->type;
		if (type == "texture_diffuse")
			current = std::to_string(currentDiffuse++);
		else if (type == "texture_specular")
			current = std::to_string(currentSpecular++);
		else if (type == "texture_normal")
			current = std::to_string(currentNormal++);
		else if (type == "texture_height")
			current = std::to_string(currentHeight++);
		shader->setInt((type + current).c_str(), index);
		glBindTexture(GL_TEXTURE_2D, textures[i]->id);
		index++;
	}

	glBindVertexArray(vertexArrayID);
	glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glActiveTexture(GL_TEXTURE0);
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

