#include "meshFilter.h"

engine::MeshFilter::MeshFilter(uint meshID) : Component(true) {
	this->meshID = meshID;
	setType();
}

engine::MeshFilter::~MeshFilter() {}

void engine::MeshFilter::setType() {
	type = typeid(*this).name();
}