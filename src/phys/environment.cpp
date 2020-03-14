#include "phys/environment.h"



environment::environment():unmovable_octree(root_aabb()), movable_octree(root_aabb())
{
  counter = 0;
}

environment::~environment()
{
  // TODO: free capsules
}

int environment::add(container *c)
{
  counter;
  containers[counter] = c;
  if(c -> collidable)
  {
    if(!c -> movable)
    {
      unmovable_octree.insert(counter, c -> to_aabb());
    }
    else
    {
      movable_octree.insert(counter, c -> to_aabb());
    }
  }
  return counter++;
}
