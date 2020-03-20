#include "phys/slicing.h"
#include "lib/log.h"
#include "phys/collisions.h"
#include <set>

glm::dvec3 gravity_vector = {0, -2, 0};

// Slices per second
const double SPS = 100;
const double BAUMGARDE_CONSTANT = 0.2;
const double COEFFICIENT_OF_RESTITUTION = 0.3;

engine::Scene *tick(engine::Scene *e)
{
  double dt = 1 / SPS;
  for(int i = 0; i < e -> gameObjects.size(); i++)
  {
    engine::GameObject *go = e -> gameObjects[i];
    if(go -> movable)
    {
      go -> add_force(gravity_vector);
      go -> tick(dt);
      if(go -> collides)
      {
        collider *c = go -> getComponent<collider>();
        aabb caabb = c -> to_aabb();
        BOOST_LOG_TRIVIAL(trace) << "Checking for colisions";
        std::set<int> collisions, partial;
        partial = e -> ctree.get_collisions(caabb);
        collisions.erase(i);
        BOOST_LOG_TRIVIAL(trace) << "Handling collision";
        for(auto collision:collisions)
        {
          glm::dvec3 axis;
          double offset;
          if(collide(e -> gameObjects[collision] -> getComponent<collider>(), c, &axis, &offset))
          {
            if(offset < 0.000001)
            {
              continue;
            }
            BOOST_LOG_TRIVIAL(trace) << "handling collision";
            double bias = -offset * BAUMGARDE_CONSTANT / dt + COEFFICIENT_OF_RESTITUTION * glm::dot(-go -> velocity, axis);
            glm::dvec3 j = axis;
            glm::dmat3 im = {{go -> im, 0, 0}, {0, go -> im, 0}, {0, 0, go -> im}};
            glm::dvec3 v = go -> velocity;
            double em = glm::dot(j, im * j);
            double lam = (-glm::dot(j, v) + bias) / em;
            glm::dvec3 dv = (im * j) * lam;
            go -> velocity += dv;
          }
          else
          {
            BOOST_LOG_TRIVIAL(trace) << "false collision";
          }
        }
      }
    }
  }
  return e;
}
