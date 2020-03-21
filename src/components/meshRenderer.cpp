#include "meshRenderer.h"
#include "meshLoader.h"
#include "meshFilter.h"
#include "graphics/gpu.h"

engine::MeshRenderer::MeshRenderer() {
	setType();
}

engine::MeshRenderer::~MeshRenderer() {
	if (initialized) {
		gpu->removeRenderer(this);
		for (int i = 0; i < meshIDs.size(); i++) {
			deleteBuffers(i);
		}
	}
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
	for (int i = 0; i < meshIDs.size(); i++) {
		vertexArrayID.push_back(0);
		vertexBufferID.push_back(0);
		elementsBufferID.push_back(0);
		outlineIndicesBufferID.push_back(0);
		Mesh* mesh = gpu->meshes[meshIDs[i]];
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
	Mesh* mesh = gpu->meshes[meshIDs[pos]];
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
		fillMeshIDs(gameObject);
		for (auto i : meshIDs) {
			materialIDs.push_back(gpu->meshes[meshIDs[i]]->materialID);
		}
		gpu->addRenderer(this);
		glContextSetup();
		initialized = true;
	}
}

void engine::MeshRenderer::fillMeshIDs(GameObject* current) {
	MeshFilter* meshFilter = current->getComponent<MeshFilter>();
	if (meshFilter != NULL) {
		meshIDs.push_back(meshFilter->meshID);
	}
	for (auto c : current->children) {
		fillMeshIDs(c);
	}
}

void engine::MeshRenderer::setType() {
	type = typeid(*this).name();
}

void engine::MeshRenderer::meshFilterRemoved(uint meshID) {
	for (int i = 0; i < meshIDs.size(); i++) {
		if (meshID == meshIDs[i]) {
			removeMeshAt(i);
			break;
		}
	}	
}

void engine::MeshRenderer::removeMeshAt(int i) {
	deleteBuffers(i);
	meshIDs.erase(meshIDs.begin() + i);
	materialIDs.erase(materialIDs.begin() + i);
}