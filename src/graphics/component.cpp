#include "component.h"
#include "objects.h"

namespace gph = graphics;

gph::Component::Component() {}

gph::Component::~Component() {}

boost::property_tree::ptree gph::Component::serialize() {}

void gph::Component::deserialize() {}

gph::Component* gph::Component::instantiate(GameObject* gameObject) {
	return NULL;
}