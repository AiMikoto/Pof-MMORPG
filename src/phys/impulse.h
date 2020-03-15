#ifndef PHYS_IMPULSE_H
#define PHYS_IMPULSE_H

#include <glm/glm.hpp>
#include <vector>

class impulse
{
public:
  void add(glm::dvec3 axis, double projection);
  glm::dvec3 get_offset();
private:
  std::vector<glm::dvec3> axi;
  std::vector<double> projections;
};

#endif
