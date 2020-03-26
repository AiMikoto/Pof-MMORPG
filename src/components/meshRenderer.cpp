#include "meshRenderer.h"
#include "graphics/model/model.h"
#include "meshFilter.h"
#include "graphics/gpu.h"
#include "lib/log.h"

engine::MeshRenderer::MeshRenderer() {
	setType();
}

engine::MeshRenderer::~MeshRenderer() {
	cleanup();
}

void engine::MeshRenderer::deleteBuffers(int i) {
	glDeleteVertexArrays(1, &vertexArrayID[i]);
	glDeleteBuffers(1, &vertexBufferID[i]);
	glDeleteBuffers(1, &elementsBufferID[i]);
	glDeleteBuffers(1, &outlineIndicesBufferID[i]);
}

boost::property_tree::ptree engine::MeshRenderer::serialize() {
	boost::property_tree::ptree node;
	return node;
}

engine::MeshRenderer* engine::MeshRenderer::deserialize(boost::property_tree::ptree node) {
	return this;
}

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
	for (int i = 0; i < materialIDs.size(); i++) {
		vertexArrayID.push_back(0);
		vertexBufferID.push_back(0);
		elementsBufferID.push_back(0);
		outlineIndicesBufferID.push_back(0);
		Mesh* mesh = gpu->models[modelID]->meshes[i];
		glGenVertexArrays(1, &vertexArrayID[i]);
		glGenBuffers(1, &vertexBufferID[i]);
		glGenBuffers(1, &elementsBufferID[i]);
		glGenBuffers(1, &outlineIndicesBufferID[i]);

		glBindVertexArray(vertexArrayID[i]);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID[i]);
		glBufferData(GL_ARRAY_BUFFER, mesh->vertices.size() * sizeof(Vertex), &mesh->vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementsBufferID[i]);
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
}

void engine::MeshRenderer::createOutline() {}

void engine::MeshRenderer::draw(Camera* camera, GLFWwindow* window, uint materialID) {
	int pos = 0;
	for (int i = 0; i < materialIDs.size(); i++) {
		if (materialID == materialIDs[i]) {
			pos = i;
			break;
		}
	}
	Mesh* mesh = gpu->models[modelID]->meshes[pos];
	Material* mat = gpu->materials[materialID];
	Shader* shader = gpu->shaders[mat->shaderID];
	glm::mat4 mvp = camera->projection(window) * camera->view() * gameObject->transform.model();
	shader->setMat4("mvp", mvp);
	glBindVertexArray(vertexArrayID[pos]);
	glDrawElements(GL_TRIANGLES, (GLsizei)mesh->indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void engine::MeshRenderer::setup() {
	if (!initialized) {
		MeshFilter* meshFilter = gameObject->getComponent<MeshFilter>();
		if (meshFilter != NULL) {
			Model* model = gpu->models[meshFilter->modelID];
			for (auto mesh : model->meshes) {
				materialIDs.push_back(mesh->materialID);
			}
			gpu->addRenderer(this);
			glContextSetup();
			initialized = true;
		}
	}
}

void engine::MeshRenderer::setType() {
	type = typeid(*this).name();
}

void engine::MeshRenderer::meshFilterRemoved() {
	cleanup();
	initialized = false;
}

void engine::MeshRenderer::cleanup() {
	if (initialized) {
		gpu->removeRenderer(this);
		for (int i = 0; i < materialIDs.size(); i++) {
			deleteBuffers(i);
		}
		materialIDs.clear();
		vertexArrayID.clear();
		vertexBufferID.clear();
		elementsBufferID.clear();
		outlineIndicesBufferID.clear();
	}
}