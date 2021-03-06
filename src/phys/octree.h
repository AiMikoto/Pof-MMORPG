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
  void insert(unsigned long long id, aabb box);
  void erase(unsigned long long id);
  std::set<unsigned long long> get_collisions(aabb box);
protected:
  void assert_partialbox(int index);
  void get_collisions_sector(int index, aabb box, std::set<unsigned long long> &ret);
  void get_collisions_h(aabb box, std::set<unsigned long long> &ret);
  octree(aabb base, int depth);
  aabb base;
  octree *children[8];
  int weight;
  std::set<unsigned long long> boxes_i;
  std::map<unsigned long long, aabb> boxes;
  std::map<unsigned long long, aabb> lazy_boxes[8];
  std::vector<unsigned long long> lazy_deletions;
  int depth;
};

#endif
