#include "graphics/culling/frustrumCulling.h"
#include "lib/log.h"

double engine::Plane::distance(glm::dvec3 point) {
	return glm::dot(normal, this->point - point);
}

engine::ViewFrustrum::ViewFrustrum() {
	planes = new Plane[frustrumValues::total];
}

engine::ViewFrustrum::~ViewFrustrum() {
	delete planes;
}

void engine::ViewFrustrum::computePoints(GLFWwindow* window, Camera* camera) {
	glm::dvec3 camPos = camera->gameObject->transform.position;
	glm::dvec3 nearCenter = camPos + camera->forward() * double(camera->nearClipDistance);
	glm::dvec3 farCenter = camPos + camera->forward() * double(camera->farClipDistance);

	int framebufferWidth, frameBufferHeight;
	glfwGetFramebufferSize(window, &framebufferWidth, &frameBufferHeight);
	double aspectRatio = double(framebufferWidth) / frameBufferHeight;

	double nearHalfWidth = tan(glm::radians(camera->fieldOfView  * aspectRatio / 2)) * double(camera->nearClipDistance);
	double nearHalfHeight = nearHalfWidth / aspectRatio;
	double farHalfWidth = tan(glm::radians(camera->fieldOfView  * aspectRatio / 2)) * double(camera->farClipDistance);
	double farHalfHeight = farHalfWidth / aspectRatio;
	if (!camera->isPerspective) {
		nearHalfWidth = farHalfWidth;
		nearHalfHeight = farHalfHeight;
	}

	namespace fv = frustrumValues;
	planes[fv::left].point = nearCenter - camera->right() * nearHalfWidth;
	planes[fv::right].point = nearCenter + camera->right() * nearHalfWidth;
	planes[fv::top].point = nearCenter + camera->up() * nearHalfHeight;
	planes[fv::bottom].point = nearCenter - camera->up() * nearHalfHeight;
	planes[fv::near].point = nearCenter;
	planes[fv::far].point = farCenter;
}

void engine::ViewFrustrum::computeNormals(Camera* camera) {
	namespace fv = frustrumValues;
	glm::dvec3 camPos = camera->gameObject->transform.position;

	planes[fv::left].normal = glm::cross(computeNormalized(camPos, planes[fv::left].point), -camera->up());
	planes[fv::right].normal = glm::cross(computeNormalized(camPos, planes[fv::right].point), camera->up());
	planes[fv::top].normal = glm::cross(computeNormalized(camPos, planes[fv::top].point), -camera->right());
	planes[fv::bottom].normal = glm::cross(computeNormalized(camPos, planes[fv::bottom].point), camera->right());
	planes[fv::near].normal = -camera->forward();
	planes[fv::far].normal = camera->forward();
}

glm::dvec3 engine::ViewFrustrum::computeNormalized(glm::dvec3 camPos, glm::dvec3 point) {
	glm::dvec3 planeVec = point - camPos;
	return  glm::normalize(planeVec);
}

bool engine::ViewFrustrum::pointInsideFrustrum(glm::dvec3 point) {
	for (int i = 0; i < frustrumValues::total; i++)
		if (planes[i].distance(point) < 0)
			return false;
	return true;
}

bool engine::ViewFrustrum::sphereInsideFrustrum(glm::dvec3 center, double radius) {
	for (int i = 0; i < frustrumValues::total; i++)
		if (planes[i].distance(center) < -radius)
			return false;
	return true;
}

bool engine::ViewFrustrum::aabbInsideFrustrum(std::vector<glm::dvec3> points) {
	for (int i = 0; i < frustrumValues::total; i++) {
		int totalIn = 0;
		for (auto p : points) {
			if (planes[i].distance(p) > 0)
				totalIn++;
		}
		if (totalIn == 0)
			return false;
	}
	return true;
}