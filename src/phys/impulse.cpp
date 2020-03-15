#include "phys/impulse.h"

void impulse::add(glm::dvec3 axis, double projection)
{
  axi.push_back(axis);
  projections.push_back(projection);
}

glm::dvec3 impulse::get_offset()
{
  glm::dvec3 offset = {0, 0, 0};
  for(int i = 0; i < axi.size(); i++)
  {
    offset += axi[i] * projections[i];
  }
  return offset;
}
