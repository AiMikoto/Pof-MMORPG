#pragma once
#include "components/camera.h"
#include "glm/gtc/type_ptr.hpp"
#include "include/glad.h"
#include "include/glfw3.h"

namespace engine {
	namespace frustrumValues {
		enum directions {
			left, right, top, bottom, near, far, total
		};
	}

	struct Plane {
		glm::dvec3 normal;
		glm::dvec3 point;

		double distance(glm::dvec3 point);
	};

	struct ViewFrustrum {
		Plane* planes;

		ViewFrustrum();
		~ViewFrustrum();

		void computePoints(GLFWwindow* window, Camera* camera);
		void computeNormals(Camera* camera);
		glm::dvec3 computeNormalized(glm::dvec3 camPos, glm::dvec3 point);
		bool pointInsideFrustrum(glm::dvec3 point);
		bool sphereInsideFrustrum(glm::dvec3 center, double radius);
		bool aabbInsideFrustrum(std::vector<glm::dvec3> points);
	};
}