#include "components/phys_collider.h"
#include "core/utils.h"

physical_collider::physical_collider(glm::dvec3 size, collider_t c_type) : collider(size, c_type)
{
  setType();
}

physical_collider::physical_collider(boost::property_tree::ptree node) : collider(node)
{
  setType();
}

physical_collider::~physical_collider()
{
}

void physical_collider::setType()
{
  type = typeid(*this).name();
  name = "physical_collider";
}

void physical_collider::setup()
{
}

boost::property_tree::ptree physical_collider::serialize()
{
  boost::property_tree::ptree tree;
  tree.put_child("size", engine::vecSerializer(size));
  tree.put("shape", static_cast<int>(c_type));
  return tree;
}
