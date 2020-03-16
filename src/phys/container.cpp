#include "phys/container.h"
#include <algorithm>

container::container(graphics::Mesh *o, container_t c_type, bool movable, bool collidable, bool collides)
{
  this -> o = o;
  this -> type = c_type;
  this -> movable = movable;
  this -> collidable = collidable;
  this -> collides = collides;
  this -> velocity = {0, 0, 0};
  this -> force_acc = {0, 0, 0};
  m = 1;
  if(collidable)
  {
    im = 0;
  }
  if(collides)
  {
    im = 1;
  }
  // if the object neither collides nor is collidable, im is never used
}

void container::add_force(glm::dvec3 force)
{
  this -> force_acc += force;
}

void container::tick(double delta)
{
  velocity += (1 / m) * force_acc * delta;
  force_acc = {0, 0, 0};
  o -> transform.position += velocity * delta;
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
  glm::dvec3 scale = this -> o -> transform.scale;
  glm::dvec3 size = this -> o -> meshScale;
  if(this -> type == caps)
  {
    // Assumptions
    // scale - x = z = radius, y = height
    // rotation - only around y axis -> no impact on aabb
    ret.minx = position.x - scale.x * size.x / 2;
    ret.maxx = position.x + scale.x * size.x / 2;
    ret.miny = position.y - scale.y * size.x / 2;
    ret.maxy = position.y + scale.y * size.x / 2;
    ret.minz = position.z - scale.z * size.x / 2;
    ret.maxz = position.z + scale.z * size.x / 2;
  }
  if(this -> type == box)
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

void get_points(container *c, glm::dvec4 *points)
{
  glm::dvec3 size = c -> o -> meshScale;
  points[0] = glm::dvec4( size.x / 2,  size.y / 2,  size.z / 2, 1);
  points[1] = glm::dvec4( size.x / 2,  size.y / 2, -size.z / 2, 1);
  points[2] = glm::dvec4( size.x / 2, -size.y / 2,  size.z / 2, 1);
  points[3] = glm::dvec4( size.x / 2, -size.y / 2, -size.z / 2, 1);
  points[4] = glm::dvec4(-size.x / 2,  size.y / 2,  size.z / 2, 1);
  points[5] = glm::dvec4(-size.x / 2,  size.y / 2, -size.z / 2, 1);
  points[6] = glm::dvec4(-size.x / 2, -size.y / 2,  size.z / 2, 1);
  points[7] = glm::dvec4(-size.x / 2, -size.y / 2, -size.z / 2, 1);
  glm::mat4x4 m = c -> o -> transform.model();
  for(int i = 0; i < 8; i++)
  {
    points[i] = m * points[i];
  }
}
