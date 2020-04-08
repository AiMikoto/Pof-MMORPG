#include "phys/octree.h"
#include <algorithm>
#include <iterator>

#define MAX_DEPTH 11

bool intersect(aabb a, aabb b)
{ // return true if there is intersection between a and b
  return a.minx <= b.maxx && a.maxx >= b.minx
      && a.miny <= b.maxy && a.maxy >= b.miny
      && a.minz <= b.maxz && a.maxz >= b.minz;
}

bool no_intersect(aabb a, aabb b)
{ // return true if there is no intersection between a and b
  return !intersect(a, b);
}

bool fully_contains(aabb a, aabb b)
{ // return true if a fully contains b
  return a.minx <= b.minx && a.maxx >= b.maxx
      && a.miny <= b.miny && a.maxy >= b.maxy
      && a.minz <= b.minz && a.maxz >= b.maxz;
}

octree::octree(aabb base)
{
  this -> base = base;
  for(int i = 0; i < 8; i++)
  {
    children[i] = NULL;
  }
  this -> depth = 0;
}

octree::octree(aabb base, int d)
{
  this -> base = base;
  for(int i = 0; i < 8; i++)
  {
    children[i] = NULL;
  }
  this -> depth = d;
}

octree::~octree()
{
  for(int i = 0; i < 8; i++)
  {
    if(children[i])
    {
      delete children[i];
    }
  }
}

void octree::insert(unsigned long long id, aabb box)
{
  if(no_intersect(box, this -> base))
  {
    return; // nothing to do
  }
  if(boxes_i.find(id) != boxes_i.end())
  {
    return; // already present
  }
  weight++;
  boxes[id] = box;
  boxes_i.insert(id);
  for(int i = 0; i < 8; i++)
  {
    lazy_boxes[i][id] = box;
  }
}

void octree::erase(unsigned long long id)
{
  if(boxes_i.find(id) == boxes_i.end())
  {
    return;
  }
  weight--;
  boxes.erase(id);
  boxes_i.erase(id);
  for(int i = 0; i < 8; i++)
  {
    lazy_boxes[i].erase(id);
  }
  lazy_deletions.push_back(id);
}

std::set<unsigned long long> octree::get_collisions(aabb box)
{
  std::set<unsigned long long> ret;
  get_collisions_h(box, ret);
  return ret;
}

void octree::assert_partialbox(int index)
{
  double midx = (this -> base.minx + this -> base.maxx) / 2;
  double midy = (this -> base.miny + this -> base.maxy) / 2;
  double midz = (this -> base.minz + this -> base.maxz) / 2;
  if(children[index] == NULL)
  {
    aabb partialbox = this -> base;
    if((index / 4) % 2 == 0)
    {
      partialbox.maxx = midx;
    }
    else
    {
      partialbox.minx = midx;
    }
    if((index / 2) % 2 == 0)
    {
      partialbox.maxy = midy;
    }
    else
    {
      partialbox.miny = midy;
    }
    if(index % 2 == 0)
    {
      partialbox.maxz = midz;
    }
    else
    {
      partialbox.minz = midz;
    }
    children[index] = new octree(partialbox, depth + 1);
  }
}

void octree::get_collisions_sector(int index, aabb box, std::set<unsigned long long> &ret)
{
  for(auto it:lazy_boxes[index])
  {
    children[index] -> insert(it.first, it.second);
  }
  lazy_boxes[index].clear();
  for(auto it:lazy_deletions)
  {
    children[index] -> erase(it);
  }
  std::set<int> difference;
  std::set_difference(children[index] -> boxes_i.begin(), children[index] -> boxes_i.end(), ret.begin(), ret.end(), std::inserter(difference, difference.end()));
  if(difference.size() != 0)
  {
    children[index] -> get_collisions_h(box, ret);
  }
}

void octree::get_collisions_h(aabb box, std::set<unsigned long long> &ret)
{
  if(weight == 0)
  {
    return;
  }
  if(no_intersect(box, this -> base))
  {
    return;
  }
  if(fully_contains(box, this -> base) || (depth == MAX_DEPTH))
  {
    for(auto it:boxes)
    {
      ret.insert(it.first);
    }
  }
  else
  {
    double midx = (this -> base.minx + this -> base.maxx) / 2;
    double midy = (this -> base.miny + this -> base.maxy) / 2;
    double midz = (this -> base.minz + this -> base.maxz) / 2;
    bool mxsplit = box.minx < midx;
    bool pxsplit = box.maxx > midx;
    bool mysplit = box.miny < midy;
    bool pysplit = box.maxy > midy;
    bool mzsplit = box.minz < midz;
    bool pzsplit = box.maxz > midz;
    if(mxsplit && mysplit && mzsplit)
    {
      int index = 0;
      assert_partialbox(index);
      get_collisions_sector(index, box, ret);
    }
    if(mxsplit && mysplit && pzsplit)
    {
      int index = 1;
      assert_partialbox(index);
      get_collisions_sector(index, box, ret);
    }
    if(mxsplit && pysplit && mzsplit)
    {
      int index = 2;
      assert_partialbox(index);
      get_collisions_sector(index, box, ret);
    }
    if(mxsplit && pysplit && pzsplit)
    {
      int index = 3;
      assert_partialbox(index);
      get_collisions_sector(index, box, ret);
    }
    if(pxsplit && mysplit && mzsplit)
    {
      int index = 4;
      assert_partialbox(index);
      get_collisions_sector(index, box, ret);
    }
    if(pxsplit && mysplit && pzsplit)
    {
      int index = 5;
      assert_partialbox(index);
      get_collisions_sector(index, box, ret);
    }
    if(pxsplit && pysplit && mzsplit)
    {
      int index = 6;
      assert_partialbox(index);
      get_collisions_sector(index, box, ret);
    }
    if(pxsplit && pysplit && pzsplit)
    {
      int index = 7;
      assert_partialbox(index);
      get_collisions_sector(index, box, ret);
    }
    lazy_deletions.clear();
  }
}
