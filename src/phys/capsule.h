#ifndef PHYS_CAPSULE_H
#define PHYS_CAPSULE_H

#include "graphics/objects.h"

enum capsule_t
{
  caps,
  box
};

class capsule
{
public:
  capsule(graphics::GameObject *o, capsule_t c_type, bool movable, bool collidable);
  ~capsule();
  capsule_t type;
  graphics::GameObject *o;
private:
  bool movable;
  bool collidable;
};

#endif // PHYS_CAPSULE_H
