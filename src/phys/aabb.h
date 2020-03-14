#ifndef PHYS_AABB_H
#define PHYS_AABB_H

#include <string>

class aabb
{
public:
  double minx;
  double maxx;
  double miny;
  double maxy;
  double minz;
  double maxz;
  std::string print();
};

aabb root_aabb();

#endif
