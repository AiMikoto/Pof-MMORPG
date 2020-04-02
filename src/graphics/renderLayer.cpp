#include "graphics/renderLayer.h"
#include "graphics/model/mesh.h"
#include "gpu.h"

engine::RenderLayer::RenderLayer(std::string modelPath, std::string materialPath, uint submeshPos) {
	this->modelPath = modelPath;
	this->materialPath = materialPath;
	this->submeshPos = submeshPos;
	setup();
}

engine::RenderLayer::~RenderLayer() {
	cleanup();
	renderers.clear();
}

void engine::RenderLayer::draw(const glm::mat4& vp) {
	Mesh* mesh = gpu->models[modelPath]->meshes[submeshPos];
	Material* mat = gpu->materials[mesh->materialPath];
	Shader* shader = gpu->shaders[mat->shaderID];
	mat->contextSetup();
	shader->setMat4("vp", vp);
	std::vector<glm::mat4> modelMatrices = getModelMatrices();
	glBindVertexArray(vertexArrayID);
	glBindBuffer(GL_ARRAY_BUFFER, instanceBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * modelMatrices.size(), &modelMatrices[0], GL_DYNAMIC_DRAW);
	for (uint i = 0; i < 4; i++) {
		glEnableVertexAttribArray(5 + i);
		glVertexAttribPointer(5 + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (const GLvoid*)(sizeof(GLfloat) * i * 4));
		glVertexAttribDivisor(5 + i, 1);
	}
	glDrawElementsInstanced(GL_TRIANGLES, (GLsizei)mesh->indices.size(), GL_UNSIGNED_INT, 0, (GLsizei)modelMatrices.size());
	glBindVertexArray(0);
}

void engine::RenderLayer::setup() {
	bindBuffers();
}

void engine::RenderLayer::bindBuffers() {
	Mesh* mesh = gpu->models[modelPath]->meshes[submeshPos];
	glGenVertexArrays(1, &vertexArrayID);
	glGenBuffers(1, &vertexBufferID);
	glGenBuffers(1, &elementsBufferID);
	glGenBuffers(1, &outlineIndicesBufferID);
	glGenBuffers(1, &instanceBufferID);

	glBindVertexArray(vertexArrayID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, mesh->vertices.size() * sizeof(Vertex), &mesh->vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementsBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->indices.size() * sizeof(uint), &mesh->indices[0], GL_STATIC_DRAW);

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

void engine::RenderLayer::cleanup() {
	glDeleteVertexArrays(1, &vertexArrayID);
	glDeleteBuffers(1, &vertexBufferID);
	glDeleteBuffers(1, &elementsBufferID);
	glDeleteBuffers(1, &outlineIndicesBufferID);
	glDeleteBuffers(1, &instanceBufferID);
}

std::vector<glm::mat4> engine::RenderLayer::getModelMatrices() {
	std::vector<glm::mat4> modelMatrices;
	for (auto r : renderers) {
		modelMatrices.push_back(r->gameObject->transform.model());
	}
	return modelMatrices;
}