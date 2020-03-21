#ifndef COMPONENTS_COLLIDER_H
#define COMPONENTS_COLLIDER_H

#include "components/component.h"
#include "phys/aabb.h"
#include "scene/objects.h"

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
  void setType();
  collider_t c_type;
  glm::dvec3 size;
};

// exclusively for box
void get_points(collider *c, glm::dvec4 *points);

#endif // COMPONENTS_CONTAINER_H
