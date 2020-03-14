#include "phys/capsule.h"

capsule::capsule(graphics::GameObject *o, capsule_t c_type, bool movable, bool collidable)
{
  this -> o = o;
  this -> type = c_type;
  this -> movable = movable;
  this -> collidable = collidable;
}

capsule::~capsule()
{
  delete o;
}
