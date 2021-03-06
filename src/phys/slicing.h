#ifndef PHYS_SLICING_H
#define PHYS_SLICING_H

#include "scene/scene.h"
#include <map>
#include <vector>
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
  std::map<unsigned long long, glm::dvec3> pos_delta;
  std::map<unsigned long long, glm::dvec3> vel_delta;
  std::map<unsigned long long, glm::dvec3> shift;
  std::map<unsigned long long, glm::dvec3> scale;
  std::map<unsigned long long, glm::dvec3> rotation;
  std::map<unsigned long long, boost::property_tree::ptree> objects;
  std::map<unsigned long long, boost::property_tree::ptree> components;
  std::vector <unsigned long long> ejections;
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

void slicer_move(unsigned long long id, glm::dvec3 pos);

void slicer_scale(unsigned long long id, glm::dvec3 scale);

void slicer_rotate(unsigned long long id, glm::dvec3 rotation);

void slicer_inject_object(engine::GameObject *go);

void slicer_inject_component(unsigned long long id, engine::Component *c);

void slicer_eject_object(unsigned long long id);

void slicer_acquire();

void slicer_release();

#endif // PHYS_SLICING_H
