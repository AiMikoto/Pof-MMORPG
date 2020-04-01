#pragma once
#include "scene/objects.h"
#include "component.h"
#include <boost/property_tree/ptree.hpp>

namespace engine {
	struct Light : public Component {
		glm::vec4 color;
		float attenuation, ambientCoefficient, coneAngle;
		glm::vec3 coneDirection;
		int lightType;

		Light();
		~Light();
		Light(glm::vec4 color, float attenuation, float ambientCoefficient, float coneAngle, glm::vec3 coneDirection, int type);
		Light(boost::property_tree::ptree node);
		boost::property_tree::ptree serialize() = 0;
		void setup() = 0;
	protected:
		void setType();
	};
}