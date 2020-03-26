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

slice_t slice(engine::Scene *e)
{
  slice_t ret;
  double dt = 1 / SPS;
  for(auto it : e -> gameObjects)
  {
    engine::GameObject *go = it.second;
    solid_object *gop = go -> getComponent<solid_object>();
    if(!gop)
    {
      continue;
    }
    else
    {
      // adding gravity vector
      gop -> force_acc += gravity_vector;
      ret.vel_delta[it.first] = gop -> im * gop -> force_acc * dt;
      gop -> force_acc = {0, 0, 0};
      ret.pos_delta[it.first] = (gop -> velocity + ret.vel_delta[it.first]) * dt;
      // emulating position change for the purpose of collision detection
      go -> transform.position += ret.pos_delta[it.first];
      collider *c = go -> getComponent<physical_collider>();
      aabb caabb = c -> to_aabb();
      BOOST_LOG_TRIVIAL(trace) << "Checking for colisions";
      std::set<unsigned long long> collisions;
      collisions = e -> ctree.get_collisions(caabb);
      collisions.erase(it.first);
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
          double bias = -offset * BAUMGARDE_CONSTANT / dt + COEFFICIENT_OF_RESTITUTION * glm::dot(-(gop -> velocity + ret.vel_delta[it.first]), axis);
          glm::dvec3 j = axis;
          glm::dmat3 im = {{gop -> im, 0, 0}, {0, gop -> im, 0}, {0, 0, gop -> im}};
          glm::dvec3 v = (gop -> velocity + ret.vel_delta[it.first]);
          double em = glm::dot(j, im * j);
          double lam = (-glm::dot(j, v) + bias) / em;
          glm::dvec3 dv = (im * j) * lam;
          ret.vel_delta[it.first] += dv;
        }
        else
        {
          BOOST_LOG_TRIVIAL(trace) << "false collision";
        }
      }
      // undo position change emulation
      go -> transform.position -= ret.pos_delta[it.first];
    }
  }
  return ret;
}

engine::Scene *apply_slice(engine::Scene *e, slice_t slice)
{
  for(auto it : slice.pos_delta)
  {
    engine::GameObject *go = e -> gameObjects[it.first];
    go -> transform.position += it.second;
  }
  for(auto it : slice.vel_delta)
  {
    engine::GameObject *go = e -> gameObjects[it.first];
    solid_object *gop = go -> getComponent<solid_object>();
    gop -> velocity += it.second;
  }
  return e;
}

engine::Scene *tick(engine::Scene *e)
{
  return apply_slice(e, slice(e));
}
