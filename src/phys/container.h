#ifndef PHYS_CONTAINER_H
#define PHYS_CONTAINER_H

#include "graphics/model.h"
#include "phys/aabb.h"

enum container_t
{
  caps,
  box,
  sphere
};

class container
{
public:
  container(graphics::Mesh *o, container_t c_type, bool movable, bool collidable, bool collides);
  ~container();
  void add_force(glm::dvec3 force);
  void tick(double delta);
  aabb to_aabb();
  container_t type;
  graphics::Mesh *o;
  glm::dvec3 velocity;
  glm::dvec3 force_acc;
  bool movable;
  bool collidable;
  bool collides;
  double m;
  double im;
};

void get_points(container *c, glm::dvec4 *points);

#endif // PHYS_CONTAINER_H
