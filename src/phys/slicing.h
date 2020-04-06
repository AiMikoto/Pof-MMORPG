#ifndef PHYS_SLICING_H
#define PHYS_SLICING_H

#include "scene/scene.h"
#include <map>
#include <boost/property_tree/ptree.hpp>

enum collision_handling_model_t
{
  CH_SI,
  CH_SM
};

extern collision_handling_model_t collision_handling_model;

class slice_t
{
public:
  slice_t();
  slice_t(boost::property_tree::ptree tree);
  void add(slice_t other);
  std::map<unsigned long long, glm::dvec3> pos_delta;
  std::map<unsigned long long, glm::dvec3> vel_delta;
  boost::property_tree::ptree encode();
  long long origin_generation;
  long long target_generation;
  std::string tag;
};

extern bool slicer_active;
extern double sps;
extern double BAUMGARDE_CONSTANT;
extern double COEFFICIENT_OF_RESTITUTION;

slice_t slice(engine::Scene *e);

engine::Scene *apply_slice(engine::Scene *e, slice_t slice);

engine::Scene *tick(engine::Scene *e);

// editor functions

void slicer_set_status(bool status);

#endif // PHYS_SLICING_H
