#ifndef COMPONENTS_PHYSICAL_H
#define COMPONENTS_PHYSICAL_H

#include "components/component.h"
#include "scene/objects.h"

class physical : public engine::Component
{
public:
  physical(bool movable, bool collides, bool collidable);
  ~physical();
  void setType();
  void initialise();
  void tick(double delta);
  void add_force(glm::dvec3 force);
  glm::dvec3 velocity;
  glm::dvec3 force_acc;
  double m;
  double im;
  bool collides;
  bool movable;
  bool collidable;
};

#endif // COMPONENTS_PHYSICAL_H
