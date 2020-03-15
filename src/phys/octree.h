#ifndef PHYS_OCTREE_H
#define PHYS_OCTREE_H

#include "phys/aabb.h"
#include <set>
#include <map>
#include <vector>

class octree
{
public:
  octree(aabb base);
  ~octree();
  void assert_split();
  void insert(int id, aabb box);
  std::set<int> get_collisions(aabb box);
protected:
  void get_collisions_h(aabb box, std::set<int> *ret);
  octree(aabb base, int depth);
  aabb base;
  octree *children[8];
  int weight;
  std::set<int> boxes_i;
  std::map<int, aabb> boxes;
  std::map<int, aabb> lazy_boxes[8];
  int depth;
};

#endif
