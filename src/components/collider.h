#ifndef COMPONENTS_COLLIDER_H
#define COMPONENTS_COLLIDER_H

#include "components/component.h"
#include "phys/aabb.h"
#include "scene/objects.h"
#include <boost/property_tree/ptree.hpp>

enum collider_t
{
  caps,
  box,
  sphere
};

class collider : public engine::Component
{
public:
  collider(glm::dvec3 size, collider_t c_type);
  ~collider();
  aabb to_aabb();
  virtual void setType() = 0;
  collider_t c_type;
  glm::dvec3 size;
  virtual boost::property_tree::ptree serialize() = 0;
  virtual void setup() = 0;
};

// exclusively for box
void get_points(collider *c, glm::dvec4 *points);

#endif // COMPONENTS_CONTAINER_H
