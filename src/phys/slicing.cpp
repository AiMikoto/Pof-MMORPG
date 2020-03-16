#include "phys/slicing.h"
#include "lib/log.h"
#include "phys/collisions.h"
#include "phys/impulse.h"

glm::dvec3 gravity_vector = {0, -2, 0};

// Slices per second
const double SPS = 100;
const double BAUMGARDE_CONSTANT = 0.2;
const double COEFFICIENT_OF_RESTITUTION = 0.3;

environment *tick(environment *e)
{
  double dt = 1 / SPS;
  for(auto it:e -> containers)
  {
    container *c = it.second;
    if(c -> movable)
    {
      c -> add_force(gravity_vector);
      c -> tick(dt);
      if(c -> collides)
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
            glm::dvec3 axis;
            double offset;
            BOOST_LOG_TRIVIAL(trace) << "box to box collision";
            if(box_box(e -> containers[collision], c, &axis, &offset))
            {
              BOOST_LOG_TRIVIAL(trace) << "handling collision";
              double bias = -offset * BAUMGARDE_CONSTANT / dt + COEFFICIENT_OF_RESTITUTION * glm::dot(-c -> velocity, axis);
              glm::dvec3 j = axis;
              glm::dmat3 im = {{c -> im, 0, 0}, {0, c -> im, 0}, {0, 0, c -> im}};
              glm::dvec3 v = c -> velocity;
              double em = glm::dot(j, im * j);
              double lam = (-glm::dot(j, v) + bias) / em;
              glm::dvec3 dv = (im * j) * lam;
              c -> velocity += dv;
            }
            else
            {
              BOOST_LOG_TRIVIAL(trace) << "false collision";
            }
          }
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
