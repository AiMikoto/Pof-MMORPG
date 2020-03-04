#pragma once
#include "objects.h"

namespace graphics {
	struct Camera :GameObject {
		glm::vec3 lookAt;
		bool isFixed;
		float cameraSpeed, rotationSpeed, defaultSpeed;
		float horizontalAngle, verticalAngle;
		float nearClipDistance, farClipDistance, fieldOfView;

		Camera();
		~Camera();
		Camera(Transform* transform);
	};
}