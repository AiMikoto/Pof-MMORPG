#pragma once
#include "objects.h"
#include "graphics_files.h"
#include "constants.h"
#include "variables.h"

namespace graphics {
	struct CameraViewport {
		float startX, startY, endX, endY; // percentages relative to the window's widht/height
		CameraViewport();
		CameraViewport(float startX, float startY, float endX, float endY);
	};

	class Camera : public GameObject {
	public:
		bool isFixed, isPerspective;
		double moveSpeed, rotationSpeed, defaultSpeed, maxSpeed, acceleration;
		float nearClipDistance, farClipDistance, fieldOfView;
		bool moveBuffer[totalCameraMovements];
		bool move, rotate;
		double yaw, pitch;
		CameraViewport viewport;
		std::vector<size_t> objectsInView;

		Camera();
		Camera(llong parentID);
		Camera(CameraViewport viewport, bool isPerspective = true, bool isFixed = false);
		Camera(Transform transform, CameraViewport viewport, bool isPerspective = true, bool isFixed = false);
		~Camera();
		void moveCamera(int direction);
		void rotateCamera(GLFWwindow* window);
		glm::vec3 lookAt();
		glm::mat4 projection(GLFWwindow* window);
		glm::mat4 view();
		void setViewport(GLFWwindow* window);
	private:
		void setup();
		glm::dvec3 forward();
		glm::dvec3 right();
		glm::dvec3 up();
		void updateRotation();
	};
}