#include "phys/octree.h"

octree::octree()
{
  for(int i = 0; i < 8; i++)
  {
    children[i] = NULL;
  }
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
}

std::vector<int> get_collisions(aabb box)
{
}
