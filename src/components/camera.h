#pragma once
#include "scene/objects.h"
#include "components/component.h"
#include "glm/gtc/type_ptr.hpp"
#include <boost/property_tree/ptree.hpp>
#include "include/glad.h"
#include "include/glfw3.h"

namespace engine {
	namespace cam {
		enum cameraMovements {
			forward, backwards, up, down, left, right,
			movements
		};
	}

	struct CameraViewport {
		float startX, startY, endX, endY; // percentages relative to the window's widht/height
		CameraViewport();
		CameraViewport(float startX, float startY, float endX, float endY);
		boost::property_tree::ptree serialize();
		CameraViewport deserialize(boost::property_tree::ptree node);
	};

	class Camera : public Component {
	public:
		bool isFixed, isPerspective;
		double moveSpeed, rotationSpeed;
		float nearClipDistance, farClipDistance, fieldOfView;
		bool moveBuffer[cam::movements];
		bool rotate;
		double yaw, pitch;
		CameraViewport viewport;
		std::vector<size_t> objectsInView;

		Camera();
		Camera(CameraViewport viewport, bool isPerspective = true, bool isFixed = false);
		Camera(Transform transform, CameraViewport viewport, bool isPerspective = true, bool isFixed = false);
		~Camera();
		void moveCamera(int direction);
		void rotateCamera(GLFWwindow* window);
		glm::vec3 lookAt();
		glm::mat4 projection(GLFWwindow* window);
		glm::mat4 view();
		void setViewport(GLFWwindow* window);
		virtual Camera* instantiate();
		boost::property_tree::ptree serialize();
		Camera* deserialize(boost::property_tree::ptree node);
		virtual void update();
		virtual void setup();
	protected:
		virtual void setType();
		glm::dvec3 forward();
		glm::dvec3 right();
		glm::dvec3 up();
		void updateRotation();
	};
}