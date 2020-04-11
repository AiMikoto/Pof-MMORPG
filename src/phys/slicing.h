#ifndef PHYS_SLICING_H
#define PHYS_SLICING_H

#include "scene/scene.h"
#include <map>
#include <vector>
#include <boost/property_tree/ptree.hpp>
#include "scene/oid.h"

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
  std::map<oid_t, glm::dvec3> pos_delta;
  std::map<oid_t, glm::dvec3> vel_delta;
  std::map<oid_t, glm::dvec3> shift;
  std::map<oid_t, glm::dvec3> scale;
  std::map<oid_t, glm::dvec3> rotation;
  std::map<oid_t, boost::property_tree::ptree> objects;
  std::map<oid_t, boost::property_tree::ptree> components;
  std::vector <oid_t> ejections;
  boost::property_tree::ptree encode();
  long long origin_generation;
  long long target_generation;
  std::string tag;
};

extern bool slicer_active;
extern double SPS;
extern double BAUMGARDE_CONSTANT;
extern double COEFFICIENT_OF_RESTITUTION;

slice_t slice(engine::Scene *e);

engine::Scene *apply_slice(engine::Scene *e, slice_t slice);

engine::Scene *tick(engine::Scene *e);

// editor functions

void slicer_set_status(bool status);

void slicer_set_sps(double val);

void slicer_move(oid_t &id, glm::dvec3 pos);

void slicer_scale(oid_t &id, glm::dvec3 scale);

void slicer_rotate(oid_t &id, glm::dvec3 rotation);

void slicer_inject_object(oid_t &id, engine::GameObject *go);

void slicer_inject_component(oid_t &id, engine::Component *c);

void slicer_eject_object(oid_t &id);

void slicer_acquire();

void slicer_release();

#endif // PHYS_SLICING_H
