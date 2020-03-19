#include "component.h"
#include "common/objects.h"

engine::Component::Component() {}

engine::Component::Component(bool allowMultiple) {
	this->allowMultiple = allowMultiple;
}

engine::Component::~Component() {}

void engine::Component::setType() {}

boost::property_tree::ptree engine::Component::serialize() {
	boost::property_tree::ptree node;
	return node;
}

void engine::Component::deserialize() {}

engine::Component* engine::Component::instantiate() {
	return NULL;
}

void engine::Component::setup() {
	initialized = true;
}