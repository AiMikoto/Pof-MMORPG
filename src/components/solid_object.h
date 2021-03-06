#ifndef COMPONENTS_SOLID_OBJECT_H
#define COMPONENTS_SOLID_OBJECT_H

#include "components/component.h"
#include "scene/objects.h"
#include <boost/property_tree/ptree.hpp>

class solid_object : public engine::Component
{
public:
  solid_object();
  solid_object(boost::property_tree::ptree node);
  ~solid_object();
  void setType();
  void add_force(glm::dvec3 force);
  boost::property_tree::ptree serialize();
  void setup();
  glm::dvec3 velocity;
  glm::dvec3 force_acc;
  double m;
  double im;
};

#endif // COMPONENTS_SOLID_OBJECT_H
