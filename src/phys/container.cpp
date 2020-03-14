#include "phys/container.h"

double dmin(double x, double y)
{
  return (x < y) ? x : y;
}

double dmax(double x, double y)
{
  return (x > y) ? x : y;
}

container::container(graphics::GameObject *o, container_t c_type, bool movable, bool collidable)
{
  this -> o = o;
  this -> type = c_type;
  this -> movable = movable;
  this -> collidable = collidable;
  this -> velocity = {0, 0, 0};
}

container::~container()
{
  delete o;
}

aabb container::to_aabb()
{
  aabb ret;
  glm::dvec3 position = this -> o -> transform.position;
  double oneeighty = 180;
  glm::dvec3 rotation = glm::eulerAngles(this -> o -> transform.rotation) * pi / oneeighty;
  glm::dvec3 scale = this -> o -> transform.scale;
  if(this -> type == caps)
  {
    // Assumptions
    // scale - x = z = radius, y = height
    // rotation - only around y axis -> no impact on aabb
    ret.minx = position.x - scale.x / 2;
    ret.maxx = position.x + scale.x / 2;
    ret.miny = position.y - scale.y / 2;
    ret.maxy = position.y + scale.y / 2;
    ret.minz = position.z - scale.z / 2;
    ret.maxz = position.z + scale.z / 2;
  }
  if(this -> type == box)
  {
    // Assumptions
    // rotation - only around x y and z axis
    glm::dvec4 points[8];
    points[0] = glm::dvec4( 0.5,  0.5,  0.5, 1);
    points[0] = glm::dvec4( 0.5,  0.5, -0.5, 1);
    points[0] = glm::dvec4( 0.5, -0.5,  0.5, 1);
    points[0] = glm::dvec4( 0.5, -0.5, -0.5, 1);
    points[0] = glm::dvec4(-0.5,  0.5,  0.5, 1);
    points[0] = glm::dvec4(-0.5,  0.5, -0.5, 1);
    points[0] = glm::dvec4(-0.5, -0.5,  0.5, 1);
    points[0] = glm::dvec4(-0.5, -0.5, -0.5, 1);
    for(int i = 0; i < 8; i++)
    {
      points[i] = o -> transform.model() * points[i];
    }
    // calculate min/max values;
    for(int i = 0; i < 8; i++)
    {
      ret.minx = dmin(ret.minx, points[i].x);
      ret.maxx = dmax(ret.maxx, points[i].x);
      ret.miny = dmin(ret.miny, points[i].y);
      ret.maxy = dmax(ret.maxy, points[i].y);
      ret.minz = dmin(ret.minz, points[i].z);
      ret.maxz = dmax(ret.maxz, points[i].z);
    }
  }
  return ret;
}
