#pragma once
#include "objects.h"
#include "graphics_files.h"
#include "constants.h"
#include "variables.h"

namespace graphics {
	struct Camera : GameObject {
		glm::vec3 lookAt;
		bool isFixed, isPerspective;
		double moveSpeed, rotationSpeed, defaultSpeed, maxSpeed, acceleration;
		float nearClipDistance, farClipDistance, fieldOfView;
		bool moveBuffer[totalCameraMovements];
		bool move, rotate;

		Camera();
		~Camera();
		Camera(Transform* transform, glm::vec3 lookAt);
		void setup();
		void moveCamera(int direction);
		void rotateCamera(GLFWwindow* window);
	};

	extern std::vector<Camera*> cameras;
}