#ifndef COMPONENTS_SOLID_OBJECT_H
#define COMPONENTS_SOLID_OBJECT_H

#include "components/component.h"
#include "scene/objects.h"

class solid_object : public engine::Component
{
public:
  solid_object();
  ~solid_object();
  void setType();
  void initialise();
  void tick(double delta);
  void add_force(glm::dvec3 force);
  glm::dvec3 velocity;
  glm::dvec3 force_acc;
  double m;
  double im;
};

#endif // COMPONENTS_SOLID_OBJECT_H
