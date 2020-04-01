#include "components/component.h"
#include "scene/objects.h"

engine::Component::Component() {
	this->gameObject = NULL;
}

engine::Component::Component(bool allowMultiple) {
	this->allowMultiple = allowMultiple;
	this->gameObject = NULL;
}

engine::Component::Component(const Component& component) {
	this->allowMultiple = component.allowMultiple;
	this->gameObject = component.gameObject;
	this->initialized = component.initialized;
}

engine::Component::~Component() {}