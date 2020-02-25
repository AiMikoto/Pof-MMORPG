#include "world_structure/object.h"

surface::surface(struct position p1, struct position p2)
{
  this -> p1 = p1;
  this -> p2 = p2;
}

entity::entity(struct position p)
{
  this -> p = p;
}
