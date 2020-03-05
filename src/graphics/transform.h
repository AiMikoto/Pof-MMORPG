#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <vector>
#include <string>

namespace graphics {
	struct GameObject;

	struct Transform {
		glm::dvec3 position, scale;
		glm::dquat rotation;
		GameObject* gameObject;

		Transform();
		~Transform();
		Transform(glm::dvec3 position, glm::dquat rotation, glm::dvec3 scale);
		glm::dvec3 forward();
		glm::dvec3 right();
		glm::dvec3 up();
	};
}
