#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <vector>
#include <string>

namespace graphics {
	struct GameObject;

	struct Transform {
		glm::vec3 position, scale;
		glm::quat rotation;
		GameObject* gameObject;

		Transform();
		~Transform();
		Transform(glm::vec3 position, glm::quat rotation, glm::vec3 scale);
		glm::vec3 forward();
		glm::vec3 right();
		glm::vec3 up();
	};
}
