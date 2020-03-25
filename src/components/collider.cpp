#include "components/collider.h"
#include <algorithm>

collider::collider(glm::dvec3 size, collider_t c_type) : Component(true)
{
  this -> c_type = c_type;
  this -> size = size;
}

collider::~collider()
{
}

aabb collider::to_aabb()
{
  aabb ret;
  glm::dvec3 position = this -> gameObject -> transform.position;
  glm::dvec3 scale = this -> gameObject -> transform.scale;
  glm::dvec3 size = this ->size;
  if(this -> c_type == sphere)
  {
    // Assumptions
    // size  - x = y = z = radius
    // rotation - around any axis -> no impact on aabb	
    ret.minx = position.x - scale.x * size.x / 2;
    ret.maxx = position.x + scale.x * size.x / 2;
    ret.miny = position.y - scale.y * size.y / 2;
    ret.maxy = position.y + scale.y * size.y / 2;
    ret.minz = position.z - scale.z * size.z / 2;
    ret.maxz = position.z + scale.z * size.z / 2;
  }
  if(this -> c_type == caps)
  {
    // Assumptions
    // size - x = z = radius, y = height
    // rotation - only around y axis -> no impact on aabb
    ret.minx = position.x - scale.x * size.x / 2;
    ret.maxx = position.x + scale.x * size.x / 2;
    ret.miny = position.y - scale.y * size.y / 2;
    ret.maxy = position.y + scale.y * size.y / 2;
    ret.minz = position.z - scale.z * size.z / 2;
    ret.maxz = position.z + scale.z * size.z / 2;
  }
  if(this -> c_type == box)
  {
    // Assumptions
    glm::dvec4 points[8];
    get_points(this, points);
    // calculate min/max values;
    ret.minx = points[0].x;
    ret.maxx = points[0].x;
    ret.miny = points[0].y;
    ret.maxy = points[0].y;
    ret.minz = points[0].z;
    ret.maxz = points[0].z;
    for(int i = 1; i < 8; i++)
    {
      ret.minx = std::min(ret.minx, points[i].x);
      ret.maxx = std::max(ret.maxx, points[i].x);
      ret.miny = std::min(ret.miny, points[i].y);
      ret.maxy = std::max(ret.maxy, points[i].y);
      ret.minz = std::min(ret.minz, points[i].z);
      ret.maxz = std::max(ret.maxz, points[i].z);
    }
  }
  return ret;
}

void get_points(collider *c, glm::dvec4 *points)
{
  glm::dvec3 size = c -> size;
  points[0] = glm::dvec4( size.x / 2,  size.y / 2,  size.z / 2, 1);
  points[1] = glm::dvec4( size.x / 2,  size.y / 2, -size.z / 2, 1);
  points[2] = glm::dvec4( size.x / 2, -size.y / 2,  size.z / 2, 1);
  points[3] = glm::dvec4( size.x / 2, -size.y / 2, -size.z / 2, 1);
  points[4] = glm::dvec4(-size.x / 2,  size.y / 2,  size.z / 2, 1);
  points[5] = glm::dvec4(-size.x / 2,  size.y / 2, -size.z / 2, 1);
  points[6] = glm::dvec4(-size.x / 2, -size.y / 2,  size.z / 2, 1);
  points[7] = glm::dvec4(-size.x / 2, -size.y / 2, -size.z / 2, 1);
  glm::mat4x4 m = c -> gameObject -> transform.model();
  for(int i = 0; i < 8; i++)
  {
    points[i] = m * points[i];
  }
}
