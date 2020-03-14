#include "phys/octree.h"

#define MAX_DEPTH 10

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

void octree::assert_split()
{
  if(children[0] == NULL)
  {
    // initialise split
    aabb partialbox;
    double midx = (this -> base.minx + this -> base.maxx) / 2;
    double midy = (this -> base.miny + this -> base.maxy) / 2;
    double midz = (this -> base.minz + this -> base.maxz) / 2;
    // split
    partialbox = this -> base;
    partialbox.maxx = midx;
    partialbox.maxy = midz;
    partialbox.maxz = midz;
    children[0] = new octree(partialbox, depth + 1);
    partialbox = this -> base;
    partialbox.maxx = midx;
    partialbox.maxy = midz;
    partialbox.minz = midz;
    children[1] = new octree(partialbox, depth + 1);
    partialbox = this -> base;
    partialbox.maxx = midx;
    partialbox.miny = midz;
    partialbox.maxz = midz;
    children[2] = new octree(partialbox, depth + 1);
    partialbox = this -> base;
    partialbox.maxx = midx;
    partialbox.miny = midz;
    partialbox.minz = midz;
    children[3] = new octree(partialbox, depth + 1);
    partialbox = this -> base;
    partialbox.minx = midx;
    partialbox.maxy = midz;
    partialbox.maxz = midz;
    children[4] = new octree(partialbox, depth + 1);
    partialbox = this -> base;
    partialbox.minx = midx;
    partialbox.maxy = midz;
    partialbox.minz = midz;
    children[5] = new octree(partialbox, depth + 1);
    partialbox = this -> base;
    partialbox.minx = midx;
    partialbox.miny = midz;
    partialbox.maxz = midz;
    children[6] = new octree(partialbox, depth + 1);
    partialbox = this -> base;
    partialbox.minx = midx;
    partialbox.miny = midz;
    partialbox.minz = midz;
    children[7] = new octree(partialbox, depth + 1);
    // apply updater
    for(auto it:boxes)
    {
      for(int i = 0; i < 8; i++)
      {
        children[i] -> insert(it.first, it.second);
      }
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
  if(children[0])
  {
    for(int i = 0; i < 8; i++)
    {
      children[i] -> insert(id, box);
    }
  }
}

std::set<int> octree::get_collisions(aabb box)
{
  std::set<int> ret;
  if(no_intersect(box, this -> base))
  {
    return ret;
  }
  if(fully_contains(box, this -> base) || depth == MAX_DEPTH)
  {
    for(auto it:boxes)
    {
      ret.insert(it.first);
    }
  }
  else
  {
    assert_split();
    for(int i = 0; i < 8; i++)
    {
      std::set<int> partial = children[i] -> get_collisions(box);
      ret.insert(partial.begin(), partial.end());
    }
  }
  return ret;
}
