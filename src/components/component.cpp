#include "components/component.h"
#include "scene/objects.h"

engine::Component::Component() {
	this->gameObject = NULL;
}

engine::Component::Component(bool allowMultiple) {
	this->allowMultiple = allowMultiple;
	this->gameObject = NULL;
}

engine::Component::~Component() {}

void engine::Component::setType() {}

boost::property_tree::ptree engine::Component::serialize() {
	boost::property_tree::ptree node;
	return node;
}

engine::Component* engine::Component::deserialize(boost::property_tree::ptree node) {
	return this;
}

engine::Component* engine::Component::instantiate() {
	return NULL;
}

void engine::Component::setup() {
	initialized = true;
}