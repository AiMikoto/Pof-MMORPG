#include "components/phys_collider.h"

physical_collider::physical_collider(glm::dvec3 size, collider_t c_type) : collider(size, c_type)
{
  setType();
}

physical_collider::~physical_collider()
{
}

void physical_collider::setType()
{
  type = typeid(*this).name();
}

void physical_collider::setup()
{
}

boost::property_tree::ptree physical_collider::serialize()
{
  boost::property_tree::ptree tree;
  return tree;
}
