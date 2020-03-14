#ifndef PHYS_CONTAINER_H
#define PHYS_CONTAINER_H

#include "graphics/objects.h"
#include "phys/aabb.h"

enum container_t
{
  caps,
  box
};

class container
{
public:
  container(graphics::GameObject *o, container_t c_type, bool movable, bool collidable);
  ~container();
  aabb to_aabb();
  container_t type;
  graphics::GameObject *o;
  glm::dvec3 velocity;
  bool movable;
  bool collidable;
};

#endif // PHYS_CONTAINER_H
