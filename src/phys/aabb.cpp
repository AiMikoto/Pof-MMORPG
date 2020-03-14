#include "phys/aabb.h"
#include <sstream>

aabb root_aabb()
{
  aabb ret;
  ret.minx = -1000;
  ret.miny = -1000;
  ret.minz = -1000;
  ret.maxx =  1000;
  ret.maxy =  1000;
  ret.maxz =  1000;
  return ret;
}

std::string aabb::print()
{
  std::stringstream ss;
  ss << "[" 
    << minx << ", " << maxx << ", "
    << miny << ", " << maxy << ", "
    << minz << ", " << maxz
    << "]";
  return ss.str();
}
