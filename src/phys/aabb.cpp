#include "phys/aabb.h"
#include <sstream>

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
