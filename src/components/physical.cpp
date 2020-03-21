


#include "components/physical.h"

physical::physical(bool movable, bool collides, bool collidable) : Component(false)
{
  velocity = {0, 0, 0};
  force_acc = {0, 0, 0};
  m = 1;
  im = 1;
  this -> collides = collides;
  this -> movable = movable;
  this -> collidable = collidable;
  setType();
}

physical::~physical()
{
}

void physical::setType()
{
  type = typeid(*this).name();
}

void physical::tick(double delta)
{
  velocity += im * force_acc * delta;
  force_acc = {0, 0, 0};
  this -> gameObject -> transform.position += velocity * delta;
}

void physical::add_force(glm::dvec3 force)
{
  this -> force_acc += force;
}
