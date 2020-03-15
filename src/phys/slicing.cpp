#include "phys/slicing.h"
#include "lib/log.h"
#include "phys/collisions.h"

glm::dvec3 gravity_vector = {0, -1, 0};

// Slices per second
const double SPS = 1;

environment *tick(environment *e)
{
  for(auto it:e -> containers)
  {
    container *c = it.second;
    if(c -> movable)
    {
      BOOST_LOG_TRIVIAL(trace) << "Updating position";
      c -> o -> transform.position += c -> velocity * (1 / SPS) + gravity_vector * (pow(1 / SPS, 2) / 2);
      BOOST_LOG_TRIVIAL(trace) << "Applying gravity vector to velocity";
      c -> velocity += gravity_vector * (1 / SPS);
      if(c -> collidable)
      {
        aabb caabb = c -> to_aabb();
        BOOST_LOG_TRIVIAL(trace) << "Checking for colisions";
        std::set<int> collisions, partial;
        partial = e -> unmovable_octree.get_collisions(caabb);
        collisions.insert(partial.begin(), partial.end());
        partial = e -> movable_octree.get_collisions(caabb);
        collisions.insert(partial.begin(), partial.end());
        collisions.erase(it.first);
        BOOST_LOG_TRIVIAL(trace) << "Handling collision";
        for(auto collision:collisions)
        {
          if((c -> type == box) && (e -> containers[collision] -> type == box))
          {
            BOOST_LOG_TRIVIAL(trace) << "box to box collision";
            box_box(c, e -> containers[collision]);
          }
          BOOST_LOG_TRIVIAL(trace) << "Updating position";
          BOOST_LOG_TRIVIAL(trace) << "Updating velocity";
        }
      }
    }
  }
  BOOST_LOG_TRIVIAL(trace) << "Generating new dynamic octree";
  e -> movable_octree = octree(root_aabb());
  for(auto it:e -> containers)
  {
    container *c = it.second;
    if(c -> movable && c -> collidable)
    {
      e -> movable_octree.insert(it.first, c -> to_aabb());
    }
  }
  return e;
}
