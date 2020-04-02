#include "components/light.h"
#include "scene/transform.h"
#include <typeinfo>

engine::Light::Light() {}

engine::Light::~Light() {}

engine::Light::Light(glm::vec4 color, float attenuation, float ambientCoefficient, float coneAngle, glm::vec3 coneDirection, int type) {
	this->color = color;
	this->attenuation = attenuation;
	this->ambientCoefficient = ambientCoefficient;
	this->coneAngle = coneAngle;
	this->coneDirection = coneDirection;
	this->type = type;
}

void engine::Light::setType() {
	type = typeid(*this).name();
	name = "Light";
}
