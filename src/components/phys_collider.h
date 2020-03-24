#ifndef COMPONENTS_PHYSICAL_COLLIDER_H
#define COMPONENTS_PHYSICAL_COLLIDER_H

#include "components/component.h"
#include "components/collider.h"
#include "scene/objects.h"

class physical_collider : public collider
{
public:
  physical_collider(glm::dvec3 size, collider_t c_type);
  ~physical_collider();
  void setType();
  void initialise();
};

#endif // COMPONENTS_PHYSICAL_COLLIDER_H