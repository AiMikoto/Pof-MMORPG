#include "phys/slicing.h"
#include "lib/log.h"

glm::dvec3 gravity_vector = {0, -1, 0};

// Slices per second
double SPS = 1;

environment *tick(environment *e)
{
  for(auto it:e -> capsules)
  {
    capsule *c = it.second;
    if(c -> movable)
    {
      BOOST_LOG_TRIVIAL(trace) << "Applying gravity vector to velocity";
      c -> velocity += gravity_vector * (1 / SPS);
      BOOST_LOG_TRIVIAL(trace) << "Updating position";
      c -> o -> transform.position += c -> velocity * (1/SPS);
      if(c -> collidable)
      {
        BOOST_LOG_TRIVIAL(trace) << "Checking for colisions";
        BOOST_LOG_TRIVIAL(trace) << "Handling collision";
        BOOST_LOG_TRIVIAL(trace) << "Updating position";
        BOOST_LOG_TRIVIAL(trace) << "Updating velocity";
      }
    }
  }
    
    
  return e;
}
