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
		bool isFixed, isPerspective;
		double moveSpeed, rotationSpeed, defaultSpeed, maxSpeed, acceleration;
		float nearClipDistance, farClipDistance, fieldOfView;
		bool moveBuffer[totalCameraMovements];
		bool move, rotate;
		double yaw, pitch;
		CameraViewport viewport;

		Camera();
		~Camera();
		Camera(CameraViewport viewport);
		Camera(Transform transform, CameraViewport viewport);
		void setup();
		void moveCamera(int direction);
		void rotateCamera(GLFWwindow* window);
		glm::vec3 lookAt();
	private:
		glm::dvec3 forward();
		glm::dvec3 right();
		glm::dvec3 up();
		void updateRotation();
	};

	extern std::vector<Camera*> cameras;
}