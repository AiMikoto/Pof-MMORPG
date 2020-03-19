#pragma once
#include "common/objects.h"
#include "component.h"

namespace engine {
	struct Light : public Component {
		glm::vec4 color;
		float attenuation, ambientCoefficient, coneAngle;
		glm::vec3 coneDirection;
		int type;

		Light();
		~Light();
		Light(glm::vec4 color, float attenuation, float ambientCoefficient, float coneAngle, glm::vec3 coneDirection, int type);
	};
}