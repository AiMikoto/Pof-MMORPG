#include "component.h"
#include "objects.h"

namespace gph = graphics;

gph::Component::Component() {}

gph::Component::Component(bool allowMultiple) {
	this->allowMultiple = allowMultiple;
}

gph::Component::~Component() {}

void gph::Component::setType() {}

boost::property_tree::ptree gph::Component::serialize() {
	boost::property_tree::ptree node;
	return node;
}

void gph::Component::deserialize() {}

gph::Component* gph::Component::instantiate() {
	return NULL;
}

void gph::Component::setup() {
	initialized = true;
}