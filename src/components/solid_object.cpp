#include "components/solid_object.h"

solid_object::solid_object() : Component(false)
{
  velocity = {0, 0, 0};
  force_acc = {0, 0, 0};
  m = 1;
  im = 1;
  setType();
}

solid_object::~solid_object()
{
}

void solid_object::setType()
{
  type = typeid(*this).name();
}

void solid_object::add_force(glm::dvec3 force)
{
  this -> force_acc += force;
}
