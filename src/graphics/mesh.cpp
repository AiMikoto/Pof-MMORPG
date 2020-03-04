#include "mesh.h"
#include "transform.h"
#include "light.h"

namespace gph = graphics;

gph::Mesh::Mesh() { }

gph::Mesh::~Mesh() {
	glDeleteBuffers(1, &positionBuffer);
	glDeleteBuffers(1, &textureBuffer);
	glDeleteBuffers(1, &normalBuffer);
	glDeleteBuffers(1, &elementBuffer);
	glDeleteBuffers(1, &outlineBuffer);
	vertices.clear();
	textures.clear();
	indices.clear();
	outlineIndices.clear();
}

void gph::Mesh::draw(glm::mat4 &modelMatrix, glm::mat4 &viewMatrix, glm::mat4 &projectionMatrix, Camera* camera, std::vector<Light*> lights,
	bool drawOutline, glm::mat4 modelOutlineMatrix, bool drawWireFrame) {

	glm::mat4 mvp = projectionMatrix * viewMatrix * modelMatrix;
	glm::mat4 outlineMVP = projectionMatrix * viewMatrix * modelOutlineMatrix;
	drawModel(mvp, modelMatrix, camera, lights);
	if (drawWireFrame) this->drawWireFrame(mvp);
	if (drawOutline) this->drawOutline(outlineMVP);
}

void gph::Mesh::drawModel(glm::mat4 &mvp, glm::mat4 &modelMatrix, Camera* camera, std::vector<Light*> lights) {}

void gph::Mesh::drawWireFrame(glm::mat4 &mvp) {}

void gph::Mesh::drawOutline(glm::mat4 &mvp) {}

void gph::Mesh::createOutline() {}

void gph::Mesh::bindBuffers() {}

bool gph::Mesh::load(std::string path) {}

void gph::Mesh::copy(Mesh* target) {}