#ifndef COMPONENTS_PHYSICAL_COLLIDER_H
#define COMPONENTS_PHYSICAL_COLLIDER_H

#include "components/component.h"
#include "components/collider.h"
#include "scene/objects.h"
#include <boost/property_tree/ptree.hpp>

class physical_collider : public collider
{
public:
  physical_collider(glm::dvec3 size, collider_t c_type);
  ~physical_collider();
  void setType();
  void initialise();
  boost::property_tree::ptree serialize() { boost::property_tree::ptree node; return node; }
};

#endif // COMPONENTS_PHYSICAL_COLLIDER_H
