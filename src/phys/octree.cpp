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

void octree::insert(int id, aabb box)
{
  if(no_intersect(box, this -> base))
  {
    return;
  }
  weight++;
  boxes[id] = box;
  boxes_i.insert(id);
  for(int i = 0; i < 8; i++)
  {
    lazy_boxes[i][id] = box;
  }
}

std::set<int> octree::get_collisions(aabb box)
{
  std::set<int> ret;
  get_collisions_h(box, &ret);
  return ret;
}

void octree::get_collisions_h(aabb box, std::set<int> *ret)
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
      ret -> insert(it.first);
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
      if(children[index] == NULL)
      {
        aabb partialbox = this -> base;
        partialbox.maxx = midx;
        partialbox.maxy = midy;
        partialbox.maxz = midz;
        children[index] = new octree(partialbox, depth + 1);
      }
      for(auto it:lazy_boxes[index])
      {
        children[index] -> insert(it.first, it.second);
      }
      lazy_boxes[index].clear();
      std::set<int> difference;
      std::set_difference(children[index] -> boxes_i.begin(), children[index] -> boxes_i.end(), ret -> begin(), ret -> end(), std::inserter(difference, difference.end()));
      if(difference.size() != 0)
      {
	children[index] -> get_collisions_h(box, ret);
      }
    }
    if(mxsplit && mysplit && pzsplit)
    {
      int index = 1;
      if(children[index] == NULL)
      {
        aabb partialbox = this -> base;
        partialbox.maxx = midx;
        partialbox.maxy = midy;
        partialbox.minz = midz;
        children[index] = new octree(partialbox, depth + 1);
      }
      for(auto it:lazy_boxes[index])
      {
        children[index] -> insert(it.first, it.second);
      }
      lazy_boxes[index].clear();
      std::set<int> difference;
      std::set_difference(children[index] -> boxes_i.begin(), children[index] -> boxes_i.end(), ret -> begin(), ret -> end(), std::inserter(difference, difference.end()));
      if(difference.size() != 0)
      {
	children[index] -> get_collisions_h(box, ret);
      }
    }
    if(mxsplit && pysplit && mzsplit)
    {
      int index = 2;
      if(children[index] == NULL)
      {
        aabb partialbox = this -> base;
        partialbox.maxx = midx;
        partialbox.miny = midy;
        partialbox.maxz = midz;
        children[index] = new octree(partialbox, depth + 1);
      }
      for(auto it:lazy_boxes[index])
      {
        children[index] -> insert(it.first, it.second);
      }
      lazy_boxes[index].clear();
      std::set<int> difference;
      std::set_difference(children[index] -> boxes_i.begin(), children[index] -> boxes_i.end(), ret -> begin(), ret -> end(), std::inserter(difference, difference.end()));
      if(difference.size() != 0)
      {
	children[index] -> get_collisions_h(box, ret);
      }
    }
    if(mxsplit && pysplit && pzsplit)
    {
      int index = 3;
      if(children[index] == NULL)
      {
        aabb partialbox = this -> base;
        partialbox.maxx = midx;
        partialbox.miny = midy;
        partialbox.minz = midz;
        children[index] = new octree(partialbox, depth + 1);
      }
      for(auto it:lazy_boxes[index])
      {
        children[index] -> insert(it.first, it.second);
      }
      lazy_boxes[index].clear();
      std::set<int> difference;
      std::set_difference(children[index] -> boxes_i.begin(), children[index] -> boxes_i.end(), ret -> begin(), ret -> end(), std::inserter(difference, difference.end()));
      if(difference.size() != 0)
      {
	children[index] -> get_collisions_h(box, ret);
      }
    }
    if(pxsplit && mysplit && mzsplit)
    {
      int index = 4;
      if(children[index] == NULL)
      {
        aabb partialbox = this -> base;
        partialbox.minx = midx;
        partialbox.maxy = midy;
        partialbox.maxz = midz;
        children[index] = new octree(partialbox, depth + 1);
      }
      for(auto it:lazy_boxes[index])
      {
        children[index] -> insert(it.first, it.second);
      }
      lazy_boxes[index].clear();
      std::set<int> difference;
      std::set_difference(children[index] -> boxes_i.begin(), children[index] -> boxes_i.end(), ret -> begin(), ret -> end(), std::inserter(difference, difference.end()));
      if(difference.size() != 0)
      {
	children[index] -> get_collisions_h(box, ret);
      }
    }
    if(pxsplit && mysplit && pzsplit)
    {
      int index = 5;
      if(children[index] == NULL)
      {
        aabb partialbox = this -> base;
        partialbox.minx = midx;
        partialbox.maxy = midy;
        partialbox.minz = midz;
        children[index] = new octree(partialbox, depth + 1);
      }
      for(auto it:lazy_boxes[index])
      {
        children[index] -> insert(it.first, it.second);
      }
      lazy_boxes[index].clear();
      std::set<int> difference;
      std::set_difference(children[index] -> boxes_i.begin(), children[index] -> boxes_i.end(), ret -> begin(), ret -> end(), std::inserter(difference, difference.end()));
      if(difference.size() != 0)
      {
	children[index] -> get_collisions_h(box, ret);
      }
    }
    if(pxsplit && pysplit && mzsplit)
    {
      int index = 6;
      if(children[index] == NULL)
      {
        aabb partialbox = this -> base;
        partialbox.minx = midx;
        partialbox.miny = midy;
        partialbox.maxz = midz;
        children[index] = new octree(partialbox, depth + 1);
      }
      for(auto it:lazy_boxes[index])
      {
        children[index] -> insert(it.first, it.second);
      }
      lazy_boxes[index].clear();
      std::set<int> difference;
      std::set_difference(children[index] -> boxes_i.begin(), children[index] -> boxes_i.end(), ret -> begin(), ret -> end(), std::inserter(difference, difference.end()));
      if(difference.size() != 0)
      {
	children[index] -> get_collisions_h(box, ret);
      }
    }
    if(pxsplit && pysplit && pzsplit)
    {
      int index = 7;
      if(children[index] == NULL)
      {
        aabb partialbox = this -> base;
        partialbox.minx = midx;
        partialbox.miny = midy;
        partialbox.minz = midz;
        children[index] = new octree(partialbox, depth + 1);
      }
      for(auto it:lazy_boxes[index])
      {
        children[index] -> insert(it.first, it.second);
      }
      lazy_boxes[index].clear();
      std::set<int> difference;
      std::set_difference(children[index] -> boxes_i.begin(), children[index] -> boxes_i.end(), ret -> begin(), ret -> end(), std::inserter(difference, difference.end()));
      if(difference.size() != 0)
      {
	children[index] -> get_collisions_h(box, ret);
      }
    }
  }
}
