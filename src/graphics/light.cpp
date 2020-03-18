#include "light.h"
#include "transform.h"

namespace gph = graphics;

gph::Light::Light() {}

gph::Light::~Light() {}

gph::Light::Light(glm::vec4 color, float attenuation, float ambientCoefficient, float coneAngle, glm::vec3 coneDirection) {
	this->color = color;
	this->attenuation = attenuation;
	this->ambientCoefficient = ambientCoefficient;
	this->coneAngle = coneAngle;
	this->coneDirection = coneDirection;
}
