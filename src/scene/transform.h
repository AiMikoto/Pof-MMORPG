#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <vector>
#include <string>
#include <boost/property_tree/ptree.hpp>

namespace engine {
	struct Transform {
		glm::dvec3 position, scale;
		glm::dquat rotation;

		Transform();
		~Transform();
		Transform(glm::dvec3 position, glm::dquat rotation, glm::dvec3 scale);
		glm::dvec3 forward();
		glm::dvec3 right();
		glm::dvec3 up();
		//rotates to certain angles (in degrees) on all 3 axis
		void rotateTo(double angle, glm::dvec3 axes);
		void rotateTo(glm::dvec3 rotationAngles);
		//rotates the current angles by a certain amount (in degrees) on all 3 axis
		void rotateBy(glm::dvec3 rotationAngles);
		void rotateBy(double angle, glm::dvec3 axes);
		void rotateBy(glm::dquat rotation);

		glm::mat4 translationMatrix();
		glm::mat4 rotationMatrix();
		glm::mat4 scaleMatrix();
		glm::mat4 model();

		boost::property_tree::ptree serialize();
		void deserialize(boost::property_tree::ptree node);
	private:
		std::pair<double, glm::dvec3> anglesToAngleAxis(glm::dvec3 rotationAngles);
	};
}
