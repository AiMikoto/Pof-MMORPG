#pragma once
#include "objects.h"
#include "constants.h"
#include "variables.h"

namespace graphics {
	struct Light : public GameObject {
		glm::vec4 color;
		float attenuation, ambientCoefficient, coneAngle;
		glm::vec3 coneDirection;
		uchar type;

		Light();
		~Light();
		Light(glm::vec4 color, float attenuation, float ambientCoefficient, float coneAngle, glm::vec3 coneDirection);
	};
}