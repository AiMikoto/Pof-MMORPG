#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <vector>
#include <string>
#include "constants.h"
#include "variables.h"

namespace graphics {
	struct Transform {
		glm::dvec3 position, scale;
		glm::dquat rotation;

		Transform();
		~Transform();
		Transform(glm::dvec3 position, glm::dquat rotation, glm::dvec3 scale);
		glm::dvec3 forward();
		glm::dvec3 right();
		glm::dvec3 up();
	};
}
