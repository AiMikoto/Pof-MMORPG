#include "meshRenderer.h"
#include "meshLoader.h"
#include "graphics/gpu.h"

engine::MeshRenderer::MeshRenderer() {
	setType();
}

engine::MeshRenderer::~MeshRenderer() {
	if (initialized) {
		gpu->removeRenderer(this);
	}
}

boost::property_tree::ptree engine::MeshRenderer::serialize() {
	boost::property_tree::ptree node;
	return node;
}

void engine::MeshRenderer::deserialize(boost::property_tree::ptree node) {}

engine::MeshRenderer* engine::MeshRenderer::instantiate() {
	return this;
}

void engine::MeshRenderer::glContextSetup() {
	if (!initialized) {
		bindBuffers();
		createOutline();
	}
}

void engine::MeshRenderer::bindBuffers() {
	Mesh* mesh = gpu->meshes[meshID];
	glGenVertexArrays(1, &vertexArrayID);
	glGenBuffers(1, &vertexBufferID);
	glGenBuffers(1, &elementsBufferID);
	glGenBuffers(1, &outlineIndicesBufferID);

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

void engine::MeshRenderer::createOutline() {}

void engine::MeshRenderer::draw(Camera* camera, GLFWwindow* window) {
	Mesh* mesh = gpu->meshes[meshID];
	Material* mat = gpu->materials[mesh->materialID];
	Shader* shader = gpu->shaders[mat->shaderID];
	glm::mat4 mvp = camera->projection(window) * camera->view() * gameObject->transform.model();
	shader->setMat4("mvp", mvp);
	glBindVertexArray(vertexArrayID);
	glDrawElements(GL_TRIANGLES, (GLsizei)mesh->indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void engine::MeshRenderer::setup() {
	if (!initialized) {
		MeshLoader* meshLoader = gameObject->getComponent<MeshLoader>();
		if (meshLoader != NULL) {
			meshID = meshLoader->meshID;
			materialID = gpu->meshes[meshID]->materialID;
			gpu->addRenderer(this);
			glContextSetup();
			initialized = true;
		}
	}
}

void engine::MeshRenderer::setType() {
	type = typeidToClassName(typeid(this).name());
}

void engine::MeshRenderer::meshLoaderRemoved() {
	initialized = false;
	gpu->removeRenderer(this);
}