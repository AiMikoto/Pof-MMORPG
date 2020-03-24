#include "phys/slicing.h"
#include "lib/log.h"
#include "phys/collisions.h"
#include "components/solid_object.h"
#include "components/phys_collider.h"
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
  printf("attempting to get component\n");
    solid_object *gop = go -> getComponent<solid_object>();
  printf("component received\n");
    if(!gop)
    {
      continue;
    }
    else
    {
      gop -> add_force(gravity_vector);
      gop -> tick(dt);
      collider *c = go -> getComponent<collider>();
      aabb caabb = c -> to_aabb();
      BOOST_LOG_TRIVIAL(trace) << "Checking for colisions";
      std::set<unsigned long long> collisions;
      collisions = e -> ctree.get_collisions(caabb);
      collisions.erase(i);
      BOOST_LOG_TRIVIAL(trace) << "Handling collisions";
      for(auto collision:collisions)
      {
        glm::dvec3 axis;
        double offset;
        physical_collider *phys = e -> gameObjects[collision] -> getComponent<physical_collider>();
        if(!phys)
        {
          continue;
        }
        if(collide(phys, c, &axis, &offset))
        {
          if(offset < 0.000001)
          {
            continue;
          }
          BOOST_LOG_TRIVIAL(trace) << "handling collision";
          double bias = -offset * BAUMGARDE_CONSTANT / dt + COEFFICIENT_OF_RESTITUTION * glm::dot(-gop -> velocity, axis);
          glm::dvec3 j = axis;
          glm::dmat3 im = {{gop -> im, 0, 0}, {0, gop -> im, 0}, {0, 0, gop -> im}};
          glm::dvec3 v = gop -> velocity;
          double em = glm::dot(j, im * j);
          double lam = (-glm::dot(j, v) + bias) / em;
          glm::dvec3 dv = (im * j) * lam;
          gop -> velocity += dv;
        }
        else
        {
          BOOST_LOG_TRIVIAL(trace) << "false collision";
        }
      }
    }
  }
  return e;
}
