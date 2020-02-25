#ifndef WORLD_STRUCTURE_OBJECT_H
#define WORLD_STRUCTURE_OBJECT_H

#include "world_structure/position.h"

class object
{
};

class surface : public object
{
public:
  surface(struct position p1, struct position p2);
  struct position p1, p2; // diagonally opposite points of a rectangle
};

class entity : public object
{
public:
  entity(struct position p);
  struct position p; // position of the object
};

#endif // WORLD_STRUCTURE_OBJECT_H
