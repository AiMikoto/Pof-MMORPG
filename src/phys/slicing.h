#ifndef PHYS_SLICING_H
#define PHYS_SLICING_H

#include "scene/scene.h"
#include <map>
#include <boost/property_tree/ptree.hpp>

class slice_t
{
public:
  slice_t();
  slice_t(boost::property_tree::ptree tree);
  std::map<unsigned long long, glm::dvec3> pos_delta;
  std::map<unsigned long long, glm::dvec3> vel_delta;
  boost::property_tree::ptree encode();
};

extern const double sps;
extern const double BAUMGARDE_CONSTANT;
extern const double COEFFICIENT_OF_RESTITUTION;

slice_t slice(engine::Scene *e);

engine::Scene *apply_slice(engine::Scene *e, slice_t slice);

engine::Scene *tick(engine::Scene *e);

#endif // PHYS_SLICING_H
