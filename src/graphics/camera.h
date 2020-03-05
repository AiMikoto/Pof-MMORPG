#pragma once
#include "objects.h"
#include "graphics_files.h"
#include "constants.h"
#include "variables.h"

namespace graphics {
	struct CameraViewport {
		int x, y, width, height;

		CameraViewport();
		~CameraViewport();
		CameraViewport(int x, int y, int width, int height);
	};

	struct Camera : GameObject {
		glm::vec3 lookAt;
		bool isFixed, isPerspective;
		double moveSpeed, rotationSpeed, defaultSpeed, maxSpeed, acceleration;
		float nearClipDistance, farClipDistance, fieldOfView;
		bool moveBuffer[totalCameraMovements];
		bool move, rotate;
		double horizontalAngle, verticalAngle;
		CameraViewport viewport;

		Camera();
		~Camera();
		Camera(CameraViewport viewport);
		Camera(Transform* transform, CameraViewport viewport);
		void setup();
		void moveCamera(int direction);
		void rotateCamera(GLFWwindow* window);
		void updateLookAt();
	};

	extern std::vector<Camera*> cameras;
}