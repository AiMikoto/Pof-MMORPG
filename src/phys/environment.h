#ifndef PHYS_ENVIRONMENT_H
#define PHYS_ENVIRONMENT_H

#include "phys/container.h"
#include <map>
#include "octree.h"

class environment
{
public:
  environment();
  ~environment();
  int add(container *c);
  std::map<int, container *> containers;
  int counter;
  octree unmovable_octree;
  octree movable_octree;
};

#endif // PHYS_ENVIRONMENT_H
