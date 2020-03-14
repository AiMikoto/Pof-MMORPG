#ifndef PHYS_OCTREE_H
#define PHYS_OCTREE_H

#include "phys/aabb.h"
#include <vector>

class octree
{
  octree();
  ~octree();
  void insert(int id, aabb box);
  std::vector<int> get_collisions(aabb box);
private:
  octree *children[8];
  int weight;
  std::vector<int> ids;
};

#endif
